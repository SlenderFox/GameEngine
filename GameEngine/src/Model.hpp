#pragma region
#pragma once
#include "Mesh.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType: int;
#pragma endregion

namespace Engine
{
	struct Model
	{
		friend class Renderer;
	private:
		bool m_loadTextures;

		std::unique_ptr<std::vector<std::unique_ptr<Mesh>>> m_meshes = nullptr;
		std::vector<Texture*> m_textures;
		std::string m_directory;

		Shader* m_shader;
		Camera* m_cameraRef;

		inline void LoadModel(const std::string* pPath) noexcept;

		inline void ProcessNode(
			const aiNode* pNode,
			const aiScene* pScene
		) noexcept;

		_NODISCARD inline
		std::unique_ptr<Mesh> ProcessMesh(
			const aiMesh* pMesh,
			const aiScene* pScene
		) noexcept;

		_NODISCARD inline
		std::vector<Texture*> LoadMaterialTextures(
			const aiMaterial* pMat,
			const aiTextureType pType,
			const Texture::TexType pTexType
		) const noexcept;

		inline void LoadTexturesToShader() const noexcept;

	public:
		Model(
			const std::string* pModelPath,
			const std::string* pShaderPath,
			Camera* pCamera = nullptr,
			const bool pLoadTextures = true
		) noexcept;

		~Model();

		void Draw(const Camera* pCamera = nullptr) const noexcept;

		constexpr void SetCameraRef(Camera* pCamera) noexcept;
		constexpr void SetShaderRef(Shader* pShader) noexcept;

		_NODISCARD Shader* GetShaderRef() const noexcept;
		_NODISCARD Mesh* GetMeshAt(const uint16_t pPos) const noexcept;
	};
}
