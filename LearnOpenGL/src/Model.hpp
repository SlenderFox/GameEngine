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
	private:
		unique_ptr<vector<unique_ptr<Mesh>>> m_meshes;
		vector<Texture*> m_loadedTextures;
		string m_directory;

		// Don't delete these, they are borrowed
		Camera* m_cameraRef;
		Shader* m_shaderRef;

		void Init(char* pPath);
		void LoadModel(string pPath);
		void ProcessNode(aiNode* pNode, const aiScene* pScene);
		unique_ptr<Mesh> ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
		vector<Texture*> LoadMaterialTextures(aiMaterial* pMat, aiTextureType pType, TexType pTexType);

	public:
		Model(char* pPath, Camera* pCamera = nullptr ,Shader* pShader = nullptr)
		 : m_cameraRef(pCamera), m_shaderRef(pShader) { Init(pPath); }

		void Destroy(bool pValidate);

		void Draw() { Draw(m_cameraRef, m_shaderRef); }
		void Draw(Camera* pCamera) { Draw(pCamera, m_shaderRef); }
		void Draw(Shader* pShader) { Draw(m_cameraRef, pShader); }
		void Draw(Camera* pCamera ,Shader* pShader);

		void SetCameraRef(Camera* pCamera) { m_cameraRef = pCamera; }
		void SetShaderRef(Shader* pShader) { m_shaderRef = pShader; }

		Mesh* GetMeshAt(unsigned int pPos);
	};
}
