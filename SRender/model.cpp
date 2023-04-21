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
		const string *_modelPath,
		const string *_shaderPath,
		camera *_camera,
		const bool _loadTextures
	)
		: m_cameraRef(_camera)
		, m_loadTextures(_loadTextures)
	{
		m_meshes = vector<mesh*>();
		m_textures = vector<texture*>();

		loadModel(_modelPath);
		m_shader = new shader(_shaderPath);
		if (m_loadTextures) loadTexturesToShader();

		debug::send("Done!", debug::type::Note, debug::impact::Small, debug::stage::End);
	}

	model::~model()
	{
		for (unsigned int i = 0; i < m_meshes.size(); ++i)
		{	delete m_meshes[i]; }

		delete m_shader;
	}

	void model::draw(const camera *_camera) const noexcept
	{
		m_shader->use();

		// If no camera is passed in, use the stored camera reference
		if (!_camera)
		{	_camera = m_cameraRef; }

		m_shader->setMat4("u_camera", _camera->getWorldToCameraMatrix());
		m_shader->setFloat3("u_viewPos", (vec3)_camera->getPosition());

		for (uint16_t i = 0; i < m_meshes.size(); ++i)
		{	getMeshAt(i)->draw(); }
	}

	inline void model::loadModel(const string *_path)
	{
		debug::send(
			"Loading model \"" + *_path + "\"",
			debug::type::Process, debug::impact::Large, debug::stage::Begin
		);

		if (!m_loadTextures)
		{
			debug::send(
				"Ignoring textures",
				debug::type::Note, debug::impact::Small, debug::stage::Mid
			);
		}

		Assimp::Importer importer;
		const aiScene *scene = importer.ReadFile(*_path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			debug::send(
				"ERROR::ASSIMP::" + string(importer.GetErrorString()),
				debug::type::Note, debug::impact::Large, debug::stage::Mid
			);
			return;
		}

		size_t last_slash = (*_path).find_last_of("\\/");
		m_directory = (*_path).substr(0, last_slash);
		processNode(scene->mRootNode, scene);
	}

	inline void model::processNode(
		const aiNode *_node,
		const aiScene *_scene
	)
	{
		// Process all the node's meshes (if any)
		for (uint32_t i = 0; i < _node->mNumMeshes; ++i)
		{
			aiMesh *mesh = _scene->mMeshes[_node->mMeshes[i]];
			m_meshes.push_back(processMesh(mesh, _scene));
		}

		// Then do the same for each of it's children
		for (uint32_t i = 0; i < _node->mNumChildren; ++i)
		{	processNode(_node->mChildren[i], _scene); }
	}

	inline mesh *model::processMesh(
		const aiMesh *_mesh,
		const aiScene *_scene
	)
	{
		// Process vertex positions, normals, and texture coordinates
		vector<vertex> vertices;
		for (uint32_t i = 0; i < _mesh->mNumVertices; ++i)
		{
			vertex vertex;
			vec3 vector;
			// Position
			vector.x = _mesh->mVertices[i].x;
			vector.y = _mesh->mVertices[i].y;
			vector.z = _mesh->mVertices[i].z;
			vertex.position = vector;
			// Normal
			if (_mesh->HasNormals())
			{
				vector.x = _mesh->mNormals[i].x;
				vector.y = _mesh->mNormals[i].y;
				vector.z = _mesh->mNormals[i].z;
				vertex.normal = vector;
			}
			// TexCoords
			vec2 vec(0, 0);
			if (_mesh->mTextureCoords[0])	// Does the mesh have texture coords
			{
				vec.x = _mesh->mTextureCoords[0][i].x;
				vec.y = _mesh->mTextureCoords[0][i].y;
			}
			vertex.texCoords = vec;

			vertices.push_back(vertex);
		}

		// Process indices
		vector<uint32_t> indices;
		for (uint32_t i = 0; i < _mesh->mNumFaces; ++i)
		{
			aiFace face = _mesh->mFaces[i];
			for (uint32_t j = 0; j < face.mNumIndices; ++j)
			{	indices.push_back(face.mIndices[j]); }
		}

		// Process material
		if (m_loadTextures && _mesh->mMaterialIndex >= 0U)
		{
			aiMaterial *material = _scene->mMaterials[_mesh->mMaterialIndex];
			vector<texture*> diffuseMaps = loadMaterialTextures(
				material,
				texture::type::diffuse
			);
			m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<texture*> specularMaps = loadMaterialTextures(
				material,
				texture::type::specular
			);
			m_textures.insert(m_textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return new mesh(&vertices, &indices);
	}

	inline vector<texture*> model::loadMaterialTextures(
		const aiMaterial *_material,
		const texture::type _texType
	) const
	{
		// Textures from this specific node being output
		vector<texture*> texturesOut;

		aiTextureType aitextype = aiTextureType_DIFFUSE;
		switch (_texType)
		{
		case texture::type::diffuse:
			aitextype = aiTextureType_DIFFUSE; break;
		case texture::type::specular:
			aitextype = aiTextureType_SPECULAR; break;
		default:
			assert(false && "Unknown texture type specified");
		};

		for (uint32_t i = 0; i < _material->GetTextureCount(aitextype); ++i)
		{
			aiString file;
			_material->GetTexture(aitextype, i, &file);
			string path = m_directory + '/' + file.C_Str();

			// First we check if the texture has already been loaded into memory
			bool loadNewTexture = true;
			for (size_t j = 0; j < texture::s_loadedTextures.size(); ++j)
			{
				// Skip if not the same texture
				if (*texture::s_loadedTextures[j] != path)
				{	continue; }

				// Texture has already been loaded into memory
				loadNewTexture = false;

				// Then check if it has been loaded into this model
				bool reuseExistingTexture = true;
				for (size_t k = 0; k < m_textures.size(); ++k)
				{
					if (*m_textures[k] == path)
					{
						reuseExistingTexture = false;
						break;
					}
				}

				if (reuseExistingTexture)
				{
					debug::send(
						"Reusing texture "
						+ std::to_string(texture::s_loadedTextures[j]->getId())
						+ ": "
						+ texture::s_loadedTextures[j]->getFile().c_str(),
						debug::type::Note,
						debug::impact::Small,
						debug::stage::Mid
					);
					texturesOut.push_back(texture::s_loadedTextures[j]);
				}

				break;
			}

			// If texture has not been loaded before, load it for the first time
			if (loadNewTexture)
			{
				texture *tex = new texture(path, _texType);
				texturesOut.push_back(tex);
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
			case texture::type::diffuse:
				name = "texture_diffuse";
				number = std::to_string(diffuseNr++);
				break;
			case texture::type::specular:
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

	constexpr void model::setCameraRef(camera *_camera) noexcept
	{	m_cameraRef = _camera; }

	constexpr void model::setShaderRef(shader *_shader) noexcept
	{	m_shader = _shader; }

	shader *model::getShaderRef() const noexcept
	{	return m_shader; }

	mesh *model::getMeshAt(const uint16_t _pos) const  noexcept
	{
		if (_pos > m_meshes.size() - 1)
		{
			debug::send("Attempting to access mesh outside array size");
			return nullptr;
		}

		return m_meshes[_pos];
	}
}
