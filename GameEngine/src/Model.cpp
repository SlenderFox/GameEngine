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
		const string *inModelPath,
		const string *inShaderPath,
		Camera *inCamera,
		const bool pLoadTextures
	) noexcept
		: m_cameraRef(inCamera)
		, m_loadTextures(pLoadTextures)
	{
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();
		m_textures = vector<Texture*>();

		LoadModel(inModelPath);
		m_shader = new Shader(inShaderPath);
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

	void Model::Draw(const Camera *inCamera) const noexcept
	{
		m_shader->Use();
		if (inCamera)
		{
			m_shader->SetMat4("u_camera", inCamera->GetWorldToCameraMatrix());
			m_shader->SetVec3("u_viewPos", (vec3)inCamera->GetPosition());
		}
		else
		{
			assert(m_cameraRef);
			m_shader->SetMat4("u_camera", m_cameraRef->GetWorldToCameraMatrix());
			m_shader->SetVec3("u_viewPos", (vec3)m_cameraRef->GetPosition());
		}

		for (uint16_t i = 0; i < m_meshes->size(); ++i)
		{ GetMeshAt(i)->Draw(); }
	}

	inline void Model::LoadModel(const string *inPath) noexcept
	{
		Debug::Send(
			"Loading model \"" + *inPath + "\"",
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
		const aiScene *scene = importer.ReadFile(*inPath, aiProcess_Triangulate | aiProcess_FlipUVs);

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
		m_directory = (*inPath).substr(0, (*inPath).find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
	}

	inline void Model::ProcessNode(
		const aiNode *inNode,
		const aiScene *inScene
	) noexcept
	{
		// Process all the node's meshes (if any)
		for (uint32_t i = 0; i < inNode->mNumMeshes; ++i)
		{
			aiMesh *mesh = inScene->mMeshes[inNode->mMeshes[i]];
			m_meshes.get()->push_back(ProcessMesh(mesh, inScene));
		}
		// Then do the same for each of it's children
		for (uint32_t i = 0; i < inNode->mNumChildren; ++i)
		{
			ProcessNode(inNode->mChildren[i], inScene);
		}
	}

	inline unique_ptr<Mesh> Model::ProcessMesh(
		const aiMesh *inMesh,
		const aiScene *inScene
	) noexcept
	{
		vector<Vertex> vertices;
		vector<uint32_t> indices;
		// Process vertex positions, normals, and texture coordinates
		for (uint32_t i = 0; i < inMesh->mNumVertices; ++i)
		{
			Vertex vertex;
			vec3 vector;
			// Position
			vector.x = inMesh->mVertices[i].x;
			vector.y = inMesh->mVertices[i].y;
			vector.z = inMesh->mVertices[i].z;
			vertex.position = vector;
			// Normal
			if (inMesh->HasNormals())
			{
				vector.x = inMesh->mNormals[i].x;
				vector.y = inMesh->mNormals[i].y;
				vector.z = inMesh->mNormals[i].z;
				vertex.normal = vector;
			}
			// TexCoords
			if (inMesh->mTextureCoords[0])	// Does the mesh have texture coords
			{
				vec2 vec;
				vec.x = inMesh->mTextureCoords[0][i].x;
				vec.y = inMesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// Process indices
		for (uint32_t i = 0; i < inMesh->mNumFaces; ++i)
		{
			aiFace face = inMesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// Process material
		if (m_loadTextures && inMesh->mMaterialIndex >= 0U)
		{
			aiMaterial *material = inScene->mMaterials[inMesh->mMaterialIndex];
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

	inline vector<Texture*> Model::LoadMaterialTextures(
		const aiMaterial *inMat,
		const aiTextureType inType,
		const Texture::TexType inTexType
	) const noexcept
	{
		// Textures from this specific node being output
		vector<Texture*> texturesOut;
		for (uint32_t i = 0; i < inMat->GetTextureCount(inType); ++i)
		{
			aiString file;
			inMat->GetTexture(inType, i, &file);
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
				Texture *tex = new Texture(path, inTexType);
				texturesOut.push_back(tex);
				Texture::s_loadedTextures.push_back(tex);
			}
		}
		return texturesOut;
	}

	inline void Model::LoadTexturesToShader() const noexcept
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
			default:
				return;
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

	constexpr void Model::SetCameraRef(Camera *inCamera) noexcept
	{ m_cameraRef = inCamera; }

	constexpr void Model::SetShaderRef(Shader *inShader) noexcept
	{ m_shader = inShader; }

	Shader *Model::GetShaderRef() const noexcept
	{ return m_shader; }

	Mesh *Model::GetMeshAt(const uint16_t inPos) const  noexcept
	{
		if (!m_meshes.get())
			return nullptr;

		if (inPos > m_meshes.get()->size() - 1)
		{
			Debug::Send("Attempting to access mesh outside array size");
			return nullptr;
		}

		return (*m_meshes.get())[inPos].get();
	}
}
