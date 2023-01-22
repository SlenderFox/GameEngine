#pragma region
#pragma once
#include "mesh.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType: int;
#pragma endregion

namespace srender
{
	struct model
	{
		friend class renderer;
	private:
		bool m_loadTextures;

		std::unique_ptr<std::vector<std::unique_ptr<mesh>>> m_meshes = nullptr;
		std::vector<texture*> m_textures;
		std::string m_directory;

		shader *m_shader;
		camera *m_cameraRef;

		inline void loadModel(const std::string *inPath) noexcept;

		inline void processNode(
			const aiNode *inNode,
			const aiScene *inScene
		) noexcept;

		_NODISCARD inline
		std::unique_ptr<mesh> processMesh(
			const aiMesh *inMesh,
			const aiScene *inScene
		) noexcept;

		_NODISCARD inline
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

		_NODISCARD shader *getShaderRef() const noexcept;
		_NODISCARD mesh *getMeshAt(const uint16_t inPos) const noexcept;
	};
}
