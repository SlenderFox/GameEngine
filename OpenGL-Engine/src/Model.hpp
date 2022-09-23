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
		static std::vector<Texture*> s_loadedTextures;

		bool m_loadTextures;

		std::unique_ptr<std::vector<std::unique_ptr<Mesh>>> m_meshes = nullptr;
		std::vector<Texture*> m_textures;
		std::string m_directory;

		// Don't delete these, they are borrowed
		Camera* m_cameraRef;
		Shader* m_shaderRef;

		void Init(char* pPath);
		void LoadModel(std::string pPath);
		void ProcessNode(aiNode* pNode, const aiScene* pScene);
		std::unique_ptr<Mesh> ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
		std::vector<Texture*> LoadMaterialTextures(aiMaterial* pMat, aiTextureType pType, TexType pTexType);
		void LoadTexturesToShader();

	public:
		Model(char* pPath, Shader* pShader, Camera* pCamera = nullptr, bool pLoadTextures = true)
		: m_shaderRef(pShader), m_cameraRef(pCamera), m_loadTextures(pLoadTextures)
		{ Init(pPath); }

		void Destroy(bool pValidate);

		void Draw(const Camera* const& pCamera = nullptr);

		void SetCameraRef(Camera* pCamera) { m_cameraRef = pCamera; }
		void SetShaderRef(Shader* pShader) { m_shaderRef = pShader; }

		Shader* GetShaderRef() const { return m_shaderRef; }

		Mesh* GetMeshAt(uint16_t pPos);
	};
}
