#include "model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/gtc/matrix_transform.hpp"
#include "assert.h"
#include "debug.hpp"

using glm::vec2;
using glm::vec3;
using std::string;
using std::vector;

namespace srender
{
	model::model(
		const string *inModelPath,
		const string *inShaderPath,
		camera *inCamera,
		const bool pLoadTextures
	) noexcept
		: m_cameraRef(inCamera)
		, m_loadTextures(pLoadTextures)
	{
		m_meshes = vector<mesh*>();
		m_textures = vector<texture*>();

		loadModel(inModelPath);
		m_shader = new shader(inShaderPath);
		if (m_loadTextures) loadTexturesToShader();

		debug::send(
			"Done!",
			debug::type::Note,
			debug::impact::Small,
			debug::stage::End
		);
	}

	model::~model()
	{
		for (unsigned int i = 0; i < m_meshes.size(); ++i)
		{ delete m_meshes[i]; }

		delete m_shader;
	}

	void model::draw(const camera *inCamera) const noexcept
	{
		m_shader->use();
		if (inCamera)
		{
			m_shader->setMat4("u_camera", inCamera->getWorldToCameraMatrix());
			m_shader->setFloat3("u_viewPos", (vec3)inCamera->getPosition());
		}
		else
		{
			assert(m_cameraRef);
			m_shader->setMat4("u_camera", m_cameraRef->getWorldToCameraMatrix());
			m_shader->setFloat3("u_viewPos", (vec3)m_cameraRef->getPosition());
		}

		for (uint16_t i = 0; i < m_meshes.size(); ++i)
		{ getMeshAt(i)->draw(); }
	}

	inline void model::loadModel(const string *inPath) noexcept
	{
		debug::send(
			"Loading model \"" + *inPath + "\"",
			debug::type::Process,
			debug::impact::Large,
			debug::stage::Begin
		);
		if (!m_loadTextures)
		{
			debug::send(
				"Ignoring textures",
				debug::type::Note,
				debug::impact::Small,
				debug::stage::Mid
			);
		}

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(*inPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			debug::send(
				"ERROR::ASSIMP::" + string(importer.GetErrorString()),
				debug::type::Note,
				debug::impact::Large,
				debug::stage::Mid
			);
			return;
		}
		m_directory = (*inPath).substr(0, (*inPath).find_last_of('/'));
		processNode(scene->mRootNode, scene);
	}

	inline void model::processNode(
		const aiNode *inNode,
		const aiScene *inScene
	) noexcept
	{
		// Process all the node's meshes (if any)
		for (uint32_t i = 0; i < inNode->mNumMeshes; ++i)
		{
			aiMesh *mesh = inScene->mMeshes[inNode->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, inScene));
		}
		// Then do the same for each of it's children
		for (uint32_t i = 0; i < inNode->mNumChildren; ++i)
		{
			processNode(inNode->mChildren[i], inScene);
		}
	}

	inline mesh *model::processMesh(
		const aiMesh *inMesh,
		const aiScene *inScene
	) noexcept
	{
		vector<vertex> vertices;
		vector<uint32_t> indices;
		// Process vertex positions, normals, and texture coordinates
		for (uint32_t i = 0; i < inMesh->mNumVertices; ++i)
		{
			vertex vertex;
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
			vector<texture*> diffuseMaps = loadMaterialTextures(
				material,
				aiTextureType_DIFFUSE,
				texture::texType::diffuse
			);
			m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<texture*> specularMaps = loadMaterialTextures(
				material,
				aiTextureType_SPECULAR,
				texture::texType::specular
			);
			m_textures.insert(m_textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return new mesh(&vertices, &indices);
	}

	inline vector<texture*> model::loadMaterialTextures(
		const aiMaterial *inMat,
		const aiTextureType inType,
		const texture::texType inTexType
	) const noexcept
	{
		// Textures from this specific node being output
		vector<texture*> texturesOut;
		for (uint32_t i = 0; i < inMat->GetTextureCount(inType); ++i)
		{
			aiString file;
			inMat->GetTexture(inType, i, &file);
			bool loadTexture = true;
			string path = (m_directory + '/' + file.C_Str());

			// First we check if the texture has already been loaded into memory
			for (size_t j = 0; j < texture::s_loadedTextures.size(); ++j)
			{
				if (texture::s_loadedTextures[j]->getFile() != path) continue;

				// If the texture has already been loaded into memory we check if it has been loaded into this model
				bool reuseTexture = true;
				for (size_t k = 0; k < m_textures.size(); ++k)
				{
					if (m_textures[k]->getFile() == path)
					{
						reuseTexture = false;
						break;
					}
				}

				if (reuseTexture)
				{
					string msg = "Reusing texture "
						+ std::to_string(texture::s_loadedTextures[j]->getId())
						+ ": "
						+ texture::s_loadedTextures[j]->getFile().data();
					debug::send(
						msg,
						debug::type::Note,
						debug::impact::Small,
						debug::stage::Mid
					);
					texturesOut.push_back(texture::s_loadedTextures[j]);
				}

				// Texture has already been loaded
				loadTexture = false;
				break;
			}

			// If texture has not been loaded before, load it for the first time
			if (loadTexture)
			{
				texture *tex = new texture(path, inTexType);
				texturesOut.push_back(tex);
				texture::s_loadedTextures.push_back(tex);
			}
		}
		return texturesOut;
	}

	inline void model::loadTexturesToShader() const noexcept
	{
		uint8_t diffuseNr = 0;
		uint8_t specularNr = 0;
		for (size_t i = 0; i < m_textures.size(); ++i)
		{
			// Retrieve texture number (the N in diffuse_textureN)
			string name;
			string number;
			switch (m_textures[i]->getType())
			{
			case texture::texType::diffuse:
				name = "texture_diffuse";
				number = std::to_string(diffuseNr++);
				break;
			case texture::texType::specular:
				name = "texture_specular";
				number = std::to_string(specularNr++);
				break;
			default:
				return;
			}

			string location = "u_material." + name + number;
			m_shader->setInt(location.c_str(), (int32_t)m_textures[i]->getId());
			string msg = {
				"Setting "
				+ location
				+ " to "
				+ std::to_string(m_textures[i]->getId())
			};
			debug::send(
				msg,
				debug::type::Note,
				debug::impact::Small,
				debug::stage::Mid
			);
		}
	}

	constexpr void model::setCameraRef(camera *inCamera) noexcept
	{ m_cameraRef = inCamera; }

	constexpr void model::setShaderRef(shader *inShader) noexcept
	{ m_shader = inShader; }

	shader *model::getShaderRef() const noexcept
	{ return m_shader; }

	mesh *model::getMeshAt(const uint16_t inPos) const  noexcept
	{
		if (inPos > m_meshes.size() - 1)
		{
			debug::send("Attempting to access mesh outside array size");
			return nullptr;
		}

		return m_meshes[inPos];
	}
}
