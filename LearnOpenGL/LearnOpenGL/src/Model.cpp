#pragma region
#include "Model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/gtc/matrix_transform.hpp"
#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif
#pragma endregion

namespace Engine
{
	Model::Model(char* pPath)
	{
		// Mesh creation
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();
		m_loadedTextures = make_unique<vector<Texture>>();
		LoadModel(pPath);
	}

	void Model::Destroy(bool pValidate)
	{
		// Destroy all meshes
		for (unsigned int i = 0; i < m_meshes->size(); ++i)
		{
			if (GetMeshAt(i) != nullptr)
				GetMeshAt(i)->Destroy(pValidate);
		}

		// Tell the unique pointers they are no longer needed
		m_meshes.release();
		m_loadedTextures.release();
	}

	void Model::Draw(Shader* pShader, Camera* pCamera)
	{
		pShader->Use();
		pShader->SetMat4("u_camera", pCamera->GetWorldToCameraMatrix());
	 	pShader->SetVec3("u_viewPos", pCamera->GetPosition());
		mat4 model = mat4(1.0f);
		pShader->SetMat4("u_model", (mat4)model);
	 	mat3 transposeInverseOfModel = mat3(glm::transpose(glm::inverse(model)));
	 	pShader->SetMat3("u_transposeInverseOfModel", (mat3)transposeInverseOfModel);
		for (unsigned int i = 0; i < m_meshes->size(); ++i)
		{
			GetMeshAt(i)->Draw(pShader);
		}
	}

	void Model::LoadModel(string pPath)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(pPath, aiProcess_Triangulate | aiProcess_FlipUVs);
		#ifdef _DEBUG
		 cout << "Loading model \"" << pPath << "\"" << endl;
		#endif

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			#ifdef _DEBUG
			 cout << "\nERROR::ASSIMP::" << importer.GetErrorString() << endl;
			#endif
			return;
		}
		m_directory = pPath.substr(0, pPath.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
		#ifdef _DEBUG
		 cout << "Done!" << endl;
		#endif
	}
	
	void Model::ProcessNode(aiNode* pNode, const aiScene* pScene)
	{
		// Process all the node's meshes (if any)
		for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
		{
			aiMesh* mesh = pScene->mMeshes[pNode->mMeshes[i]];
			m_meshes.get()->push_back(make_unique<Mesh>(ProcessMesh(mesh, pScene)));
		}
		// Then do the same for each of it's children
		for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
		{
			ProcessNode(pNode->mChildren[i], pScene);
		}
	}

	Mesh Model::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
		vector<Texture> textures;
		// Process vertex positions, normals, and texture coordinates
		for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
		{
			Vertex vertex;
			vec3 vector;
			// Position
			vector.x = pMesh->mVertices[i].x;
			vector.y = pMesh->mVertices[i].y;
			vector.z = pMesh->mVertices[i].z;
			vertex.position = vector;
			// Normal
			if (pMesh->HasNormals())
			{
				vector.x = pMesh->mNormals[i].x;
				vector.y = pMesh->mNormals[i].y;
				vector.z = pMesh->mNormals[i].z;
				vertex.normal = vector;
			}
			// TexCoords
			if (pMesh->mTextureCoords[0])	// Does the mesh have texture coords
			{
				vec2 vec;
				vec.x = pMesh->mTextureCoords[0][i].x;
				vec.y = pMesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else
				vertex.texCoords = vec2(0.0f, 0.0f);
				
			vertices.push_back(vertex);
		}
		// Process indices
		for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
		{
			aiFace face = pMesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; ++j)
				indices.push_back(face.mIndices[j]);
		}

		// Process material
		if (pMesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = pScene->mMaterials[pMesh->mMaterialIndex];
			vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TexType::diffuse);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TexType::specular);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return Mesh(vertices, indices, textures);
	}

	vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMat, aiTextureType pType, TexType pTexType)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < pMat->GetTextureCount(pType); ++i)
		{
			aiString file;
			pMat->GetTexture(pType, i, &file);
			bool skip = false;
			for (unsigned int j = 0; j < m_loadedTextures->size(); ++j)
			{
				if (std::strcmp(GetTextureAt(j)->m_file.data(), file.C_Str()) == 0)
				{
					textures.push_back(*GetTextureAt(j));
					skip = true;
					break;
				}
			}
			if (!skip)
			{
				#ifdef _DEBUG
				 cout << "\xC0";
				#endif
				string path = m_directory + '/' + string(file.C_Str());
				Texture texture;
				texture.m_id = Texture::LoadTexture(path.c_str());
				texture.m_type = pTexType;
				texture.m_file = file.C_Str();
				textures.push_back(texture);
				m_loadedTextures->push_back(texture);
			}
		}
		return textures;
	}

	#pragma region Getters
	Mesh* Model::GetMeshAt(unsigned int pPos)
	{
		if (m_meshes.get() == nullptr)
			return nullptr;

		if (pPos > m_meshes.get()->size() - 1)
		{
			#ifdef _DEBUG
			 cout << "Attempting to access mesh outside array size\n";
			#endif
			return nullptr;
		}

		return (*m_meshes.get())[pPos].get();
	}

	Texture* Model::GetTextureAt(unsigned int pPos)
	{
		if (m_loadedTextures.get() == nullptr)
			return nullptr;

		if (pPos > m_loadedTextures.get()->size() - 1)
		{
			#ifdef _DEBUG
			 cout << "Attempting to access mesh outside array size\n";
			#endif
			return nullptr;
		}

		return &(*m_loadedTextures.get())[pPos];
	}
	#pragma endregion
}
