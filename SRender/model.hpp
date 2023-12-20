#pragma once
#include "camera.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "mesh.hpp"
#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"

#ifndef _NODISCARD
#define _NODISCARD [[nodiscard]]
#endif

namespace srender
{
struct model
{
private:
	std::vector<mesh*> m_meshes;
	std::vector<texture*> m_textures;

	shader *m_shader;

	void loadModel(
		const std::string *_path,
		const bool _loadTextures
	);

	void processNode(
		const aiNode *_node,
		const aiScene *_scene,
		const std::string _directory,
		const bool _loadTextures
	);

	_NODISCARD mesh *processMesh(
		const aiMesh *_mesh,
		const aiScene *_scene,
		const std::string _directory,
		const bool _loadTextures
	);

	_NODISCARD std::vector<texture*> loadMaterialTextures(
		const aiMaterial *_material,
		const texture::type _texType,
		const std::string _directory,
		const bool _loadTextures
	) const;

	void loadTexturesToShader() const noexcept;

public:
	model(
		const std::string *_modelPath,
		const std::string *_shaderPath,
		const bool _loadTextures = true
	);

	~model();

	void draw(const camera *_camera) const noexcept;

	_NODISCARD shader *getShaderRef() const noexcept;
	_NODISCARD mesh *getMeshAt(const uint16_t _pos) const noexcept;
};
}
