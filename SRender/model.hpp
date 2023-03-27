#pragma once
#include "mesh.hpp"
#include "assimp/material.h"
#include "assimp/scene.h"
#include "assimp/mesh.h"

namespace srender
{
	struct model
	{
		friend class renderer;
	private:
		std::vector<mesh*> m_meshes;
		std::vector<texture*> m_textures;
		std::string m_directory;

		shader *m_shader;
		camera *m_cameraRef;

		bool m_loadTextures;

		inline void loadModel(const std::string *inPath) noexcept;

		inline void processNode(
			const aiNode *inNode,
			const aiScene *inScene
		) noexcept;

		[[nodiscard]] inline
		mesh *processMesh(
			const aiMesh *inMesh,
			const aiScene *inScene
		) noexcept;

		[[nodiscard]] inline
		std::vector<texture*> loadMaterialTextures(
			const aiMaterial *inMat,
			const aiTextureType inType,
			const texture::texType inTexType
		) const noexcept;

		inline void loadTexturesToShader() const noexcept;

	public:
		model(
			const std::string *inModelPath,
			const std::string *inShaderPath,
			camera *inCamera = nullptr,
			const bool inLoadTextures = true
		) noexcept;

		~model();

		void draw(const camera *inCamera = nullptr) const noexcept;

		constexpr void setCameraRef(camera *inCamera) noexcept;
		constexpr void setShaderRef(shader *inShader) noexcept;

		[[nodiscard]] shader *getShaderRef() const noexcept;
		[[nodiscard]] mesh *getMeshAt(const uint16_t inPos) const noexcept;
	};
}
