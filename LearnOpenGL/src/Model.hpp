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
	public:
		Model(char* pPath) { Init(pPath); }
		Model(char* pPath, Camera* pCamera)
		 : m_cameraRef(pCamera) { Init(pPath); }
		Model(char* pPath, Shader* pShader)
		 : m_shaderRef(pShader) { Init(pPath); }
		Model(char* pPath, Camera* pCamera ,Shader* pShader)
		 : m_cameraRef(pCamera), m_shaderRef(pShader) { Init(pPath); }
		void Destroy(bool pValidate);

		void Init(char* pPath);

		void Draw() { Draw(m_cameraRef, m_shaderRef); }
		void Draw(Camera* pCamera) { Draw(pCamera, m_shaderRef); }
		void Draw(Shader* pShader) { Draw(m_cameraRef, pShader); }
		void Draw(Camera* pCamera ,Shader* pShader);

		void SetCameraRef(Camera* pCamera) { m_cameraRef = pCamera; }
		void SetShaderRef(Shader* pShader) { m_shaderRef = pShader; }

		/**
		 * @brief Get a pointer to the mesh object at a given position
		 * 
		 * @param pPos The position in the array of the mesh
		 * @return Mesh* The pointer to the mesh object
		 */
		Mesh* GetMeshAt(unsigned int pPos);

	private:
		void LoadModel(string pPath);
		void ProcessNode(aiNode* pNode, const aiScene* pScene);
		unique_ptr<Mesh> ProcessMesh(aiMesh* pMesh, const aiScene* pScene);
		vector<Texture> LoadMaterialTextures(aiMaterial* pMat, aiTextureType pType, TexType pTexType);

		unique_ptr<vector<unique_ptr<Mesh>>> m_meshes;
		vector<Texture> m_loadedTextures;
		string m_directory;

		// Don't delete these, they are borrowed
		Camera* m_cameraRef;
		Shader* m_shaderRef;
	};
}
