#pragma region
#include "Model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/gtc/matrix_transform.hpp"
#include "assert.h"
#include "Debug.hpp"

using glm::vec2;
using glm::vec3;
using std::string;
using std::vector;
using std::unique_ptr;
using std::make_unique;
#pragma endregion

namespace Engine
{
	Model::Model(
		const string* pModelPath,
		const string* pShaderPath,
		Camera* pCamera,
		const bool pLoadTextures)
		: m_cameraRef(pCamera)
		, m_loadTextures(pLoadTextures)
	{
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();
		m_textures = vector<Texture*>();

		LoadModel(pModelPath);
		m_shader = new Shader(pShaderPath);
		if (m_loadTextures) LoadTexturesToShader();

		Debug::Send(
			"Done!",
			Debug::Type::Note,
			Debug::Impact::Small,
			Debug::Stage::End
		);
	}

	Model::~Model()
	{
		delete m_shader;
		m_meshes.get()->clear();
		m_meshes.release();
	}

	void Model::Draw(const Camera* pCamera) const noexcept
	{
		m_shader->Use();
		if (pCamera)
		{
			m_shader->SetMat4("u_camera", pCamera->GetWorldToCameraMatrix());
			m_shader->SetVec3("u_viewPos", (vec3)pCamera->GetPosition());
		}
		else
		{
			assert(m_cameraRef);
			m_shader->SetMat4("u_camera", m_cameraRef->GetWorldToCameraMatrix());
			m_shader->SetVec3("u_viewPos", (vec3)m_cameraRef->GetPosition());
		}
		for (uint16_t i = 0; i < m_meshes->size(); ++i)
		{
			GetMeshAt(i)->Draw();
		}
	}

	void Model::LoadModel(const string* pPath)
	{
		Debug::Send(
			"Loading model \"" + *pPath + "\"",
			Debug::Type::Process,
			Debug::Impact::Large,
			Debug::Stage::Begin
		);
		if (!m_loadTextures)
		{
			Debug::Send(
				"Ignoring textures",
				Debug::Type::Note,
				Debug::Impact::Small,
				Debug::Stage::Mid
			);
		}

		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(*pPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			Debug::Send(
				"ERROR::ASSIMP::" + string(importer.GetErrorString()),
				Debug::Type::Note,
				Debug::Impact::Large,
				Debug::Stage::Mid
			);
			return;
		}
		m_directory = (*pPath).substr(0, (*pPath).find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	void Model::ProcessNode(
		const aiNode* pNode,
		const aiScene* pScene) noexcept
	{
		// Process all the node's meshes (if any)
		for (uint32_t i = 0; i < pNode->mNumMeshes; ++i)
		{
			aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_meshes.get()->push_back(ProcessMesh(mesh, pScene));
		}
		// Then do the same for each of it's children
		for (uint32_t i = 0; i < pNode->mNumChildren; ++i)
		{
			ProcessNode(pNode->mChildren[i], pScene);
		}
	}

	unique_ptr<Mesh> Model::ProcessMesh(
		const aiMesh* pMesh,
		const aiScene* pScene) noexcept
	{
		vector<Vertex> vertices;
		vector<uint32_t> indices;
		// Process vertex positions, normals, and texture coordinates
		for (uint32_t i = 0; i < pMesh->mNumVertices; ++i)
		{
			Vertex vertex;
			vec3 vector;
			// Position
			vector.x = pMesh->mVertices[i].x;
			vector.y = pMesh->mVertices[i].y;
			vector.z = pMesh->mVertices[i].z;
			vertex.position = vector;
			// Normal
			if (pMesh->HasNormals())
			{
				vector.x = pMesh->mNormals[i].x;
				vector.y = pMesh->mNormals[i].y;
				vector.z = pMesh->mNormals[i].z;
				vertex.normal = vector;
			}
			// TexCoords
			if (pMesh->mTextureCoords[0])	// Does the mesh have texture coords
			{
				vec2 vec;
				vec.x = pMesh->mTextureCoords[0][i].x;
				vec.y = pMesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// Process indices
		for (uint32_t i = 0; i < pMesh->mNumFaces; ++i)
		{
			aiFace face = pMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// Process material
		if (m_loadTextures && pMesh->mMaterialIndex >= 0U)
		{
			aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
			vector<Texture*> diffuseMaps = LoadMaterialTextures(
				material,
				aiTextureType_DIFFUSE,
				Texture::TexType::Diffuse
			);
			m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<Texture*> specularMaps = LoadMaterialTextures(
				material,
				aiTextureType_SPECULAR,
				Texture::TexType::Specular
			);
			m_textures.insert(m_textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return make_unique<Mesh>(&vertices, &indices);
	}

	vector<Texture*> Model::LoadMaterialTextures(
		const aiMaterial* pMat,
		const aiTextureType pType,
		const Texture::TexType pTexType) noexcept
	{
		// Textures from this specific node being output
		vector<Texture*> texturesOut;
		for (uint32_t i = 0; i < pMat->GetTextureCount(pType); ++i)
		{
			aiString file;
			pMat->GetTexture(pType, i, &file);
			bool loadTexture = true;
			string path = (m_directory + '/' + file.C_Str());

			// First we check if the texture has already been loaded into memory
			for (size_t j = 0; j < Texture::s_loadedTextures.size(); ++j)
			{
				if (Texture::s_loadedTextures[j]->GetFile() != path) continue;

				// If the texture has already been loaded into memory we check if it has been loaded into this model
				bool reuseTexture = true;
				for (size_t k = 0; k < m_textures.size(); ++k)
				{
					if (m_textures[k]->GetFile() == path)
					{
						reuseTexture = false;
						break;
					}
				}

				if (reuseTexture)
				{
					string msg = "Reusing texture "
						+ std::to_string(Texture::s_loadedTextures[j]->GetId())
						+ ": "
						+ Texture::s_loadedTextures[j]->GetFile().data();
					Debug::Send(
						msg,
						Debug::Type::Note,
						Debug::Impact::Small,
						Debug::Stage::Mid
					);
					texturesOut.push_back(Texture::s_loadedTextures[j]);
				}

				// Texture has already been loaded
				loadTexture = false;
				break;
			}

			// If texture has not been loaded before, load it for the first time
			if (loadTexture)
			{
				Texture* tex = new Texture(path, pTexType);
				texturesOut.push_back(tex);
				Texture::s_loadedTextures.push_back(tex);
			}
		}
		return texturesOut;
	}

	void Model::LoadTexturesToShader() const noexcept
	{
		uint8_t diffuseNr = 0;
		uint8_t specularNr = 0;
		for (size_t i = 0; i < m_textures.size(); ++i)
		{
			// Retrieve texture number (the N in diffuse_textureN)
			string name;
			string number;
			switch (m_textures[i]->GetType())
			{
			case Texture::TexType::Diffuse:
				name = "texture_diffuse";
				number = std::to_string(diffuseNr++);
				break;
			case Texture::TexType::Specular:
				name = "texture_specular";
				number = std::to_string(specularNr++);
				break;
			}

			string location = "u_material." + name + number;
			m_shader->SetInt(location.c_str(), (int32_t)m_textures[i]->GetId());
			string msg = {
				"Setting "
				+ location
				+ " to "
				+ std::to_string(m_textures[i]->GetId())
			};
			Debug::Send(
				msg,
				Debug::Type::Note,
				Debug::Impact::Small,
				Debug::Stage::Mid
			);
		}
	}

	Mesh* Model::GetMeshAt(const uint16_t pPos) const  noexcept
	{
		if (!m_meshes.get())
			return nullptr;

		if (pPos > m_meshes.get()->size() - 1)
		{
			Debug::Send("Attempting to access mesh outside array size");
			return nullptr;
		}

		return (*m_meshes.get())[pPos].get();
	}
}
