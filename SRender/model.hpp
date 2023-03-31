#pragma once
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
		std::string m_directory;

		shader *m_shader;
		camera *m_cameraRef;

		bool m_loadTextures;

		inline void loadModel(const std::string *_path) noexcept;

		inline void processNode(
			const aiNode *_node,
			const aiScene *_scene
		) noexcept;

		_NODISCARD inline mesh *processMesh(
			const aiMesh *_mesh,
			const aiScene *_scene
		) noexcept;

		_NODISCARD inline std::vector<texture*> loadMaterialTextures(
			const aiMaterial *_material,
			const aiTextureType _type,
			const texture::texType _texType
		) const noexcept;

		inline void loadTexturesToShader() const noexcept;

	public:
		model(
			const std::string *_modelPath,
			const std::string *_shaderPath,
			camera *_camera = nullptr,
			const bool _loadTextures = true
		) noexcept;

		~model();

		void draw(const camera *_camera = nullptr) const noexcept;

		constexpr void setCameraRef(camera *_camera) noexcept;
		constexpr void setShaderRef(shader *_shader) noexcept;

		_NODISCARD shader *getShaderRef() const noexcept;
		_NODISCARD mesh *getMeshAt(const uint16_t _pos) const noexcept;
	};
}
