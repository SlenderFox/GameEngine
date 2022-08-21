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
		Model(char* pPath);
		void Destroy(bool pValidate);

		void Draw(Shader* pShader, Camera* pCamera);

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
	};
}
