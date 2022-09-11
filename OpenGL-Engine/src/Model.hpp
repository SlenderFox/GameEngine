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
	// Static
		static std::vector<Texture*> s_loadedTextures;

	// Member
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
		Model(char* pPath, Shader* pShader, Camera* pCamera = nullptr)
		: m_shaderRef(pShader), m_cameraRef(pCamera) { Init(pPath); }

		void Destroy(bool pValidate);

		void Draw() { Draw(m_shaderRef, m_cameraRef); }
		void Draw(Shader* pShader) { Draw(pShader, m_cameraRef); }
		void Draw(Camera* pCamera) { Draw(m_shaderRef, pCamera); }
		void Draw(Shader* pShader, Camera* pCamera);

		void SetCameraRef(Camera* pCamera) { m_cameraRef = pCamera; }
		void SetShaderRef(Shader* pShader) { m_shaderRef = pShader; }

		Shader* GetShaderRef() const { return m_shaderRef; }

		Mesh* GetMeshAt(unsigned int pPos);
	};
}
