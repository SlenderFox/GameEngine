#pragma region
#pragma once
#include "Mesh.hpp"

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;
#pragma endregion

namespace Engine
{
	class Model
	{
		friend class Renderer;
	private:
		static std::vector<Texture*> s_loadedTextures;	// Memory handled by renderer

		bool m_loadTextures;

		std::unique_ptr<std::vector<std::unique_ptr<Mesh>>> m_meshes = nullptr;
		std::vector<Texture*> m_textures;
		std::string m_directory;

		Shader* m_shader;
		Camera* m_cameraRef;

		void Init(const std::string& pModelPath, const std::string& pShaderPath);
		void LoadModel(const std::string& pPath);
		void ProcessNode(aiNode* pNode, const aiScene* pScene) noexcept;
		std::unique_ptr<Mesh> ProcessMesh(aiMesh* pMesh, const aiScene* pScene) noexcept;
		std::vector<Texture*> LoadMaterialTextures(aiMaterial* pMat, aiTextureType pType, TexType pTexType) noexcept;
		void LoadTexturesToShader() const noexcept;

	public:
		Model(const std::string& pModelPath, const std::string& pShaderPath,
		 Camera* pCamera = nullptr, bool pLoadTextures = true);
		~Model();

		void Draw(const Camera* const& pCamera = nullptr) const noexcept;

		constexpr void SetCameraRef(Camera* pCamera) noexcept { m_cameraRef = pCamera; }
		constexpr void SetShaderRef(Shader* pShader) noexcept { m_shader = pShader; }

		constexpr Shader* const& GetShaderRef() const noexcept { return m_shader; }
		Mesh* GetMeshAt(uint16_t pPos) const noexcept;
	};
}
