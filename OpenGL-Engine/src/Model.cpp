#pragma region
#include "Model.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "glm/gtc/matrix_transform.hpp"
#pragma endregion

namespace Engine
{
	// Static
	vector<Texture*> Model::s_loadedTextures = vector<Texture*>();

	// Member
	void Model::Init(char* pPath)
	{
		// Mesh creation
		m_meshes = make_unique<vector<unique_ptr<Mesh>>>();
		m_textures = vector<Texture*>();
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

		// Tell the unique pointer it is no longer needed
		m_meshes.release();
	}

	void Model::Draw(Shader* pShader, Camera* pCamera)
	{
		if (pShader == nullptr || pCamera == nullptr)
		{
			#ifdef _DEBUG
			 cout << "Drawing failed, either shader or camera are null" << endl;
			#endif
			return;
		}
		pShader->Use();
		pShader->SetMat4("u_camera", pCamera->GetWorldToCameraMatrix());
	 	pShader->SetVec3("u_viewPos", pCamera->GetPosition());
		for (unsigned int i = 0; i < m_meshes->size(); ++i)
		{
			GetMeshAt(i)->Draw();
		}
	}

	void Model::LoadModel(string pPath)
	{
		#ifdef _DEBUG
		 cout << "Loading model \"" << pPath << "\"" << endl;
		#endif
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(pPath, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			#ifdef _DEBUG
			 cout << "\nERROR::ASSIMP::" << importer.GetErrorString() << endl;
			#endif
			return;
		}
		m_directory = pPath.substr(0, pPath.find_last_of('/'));
		ProcessNode(scene->mRootNode, scene);
		LoadTexturesToShader();

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
			m_meshes.get()->push_back(ProcessMesh(mesh, pScene));
		}
		// Then do the same for each of it's children
		for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
		{
			ProcessNode(pNode->mChildren[i], pScene);
		}
	}

	unique_ptr<Mesh> Model::ProcessMesh(aiMesh* pMesh, const aiScene* pScene)
	{
		vector<Vertex> vertices;
		vector<unsigned int> indices;
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
			vector<Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TexType::diffuse);
			m_textures.insert(m_textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TexType::specular);
			m_textures.insert(m_textures.end(), specularMaps.begin(), specularMaps.end());
		}

		return make_unique<Mesh>(vertices, indices);
	}

	vector<Texture*> Model::LoadMaterialTextures(aiMaterial* pMat, aiTextureType pType, TexType pTexType)
	{
		// Textures from this specific node being output
		vector<Texture*> texturesOut;
		for (unsigned int i = 0; i < pMat->GetTextureCount(pType); ++i)
		{
			aiString file;
			pMat->GetTexture(pType, i, &file);
			bool loadTexture = true;
			// Compares to all currently loaded textures
			for (unsigned int j = 0; j < s_loadedTextures.size(); ++j)
			{
				// If texture is already in the loaded textures vector
				if (std::strcmp(s_loadedTextures[j]->GetFile().data(), (m_directory + '/' + file.C_Str()).data()) == 0)
				{
					bool reuseTexture = true;
					for (unsigned int k = 0; k < m_textures.size(); ++k)
					{
						// If the texture has already been loaded into this model, don't bother reloading it
						if (std::strcmp(m_textures[k]->GetFile().data(), (m_directory + '/' + file.C_Str()).data()) == 0)
						{
							reuseTexture = false;
							break;
						}
					}
					// If the texture has not been loaded into this model, reuse it
					if (reuseTexture)
					{
						#ifdef _DEBUG
						 cout << " \xC3Reusing " << s_loadedTextures[j]->GetFile().data() << endl;
						#endif
						texturesOut.push_back(s_loadedTextures[j]);
					}
					
					loadTexture = false;
					break;
				}
			}
			// If texture has not been loaded before, load it for the first time
			if (loadTexture)
			{
				#ifdef _DEBUG
				 cout << " \xD8";
				#endif
				Texture* tex = new Texture(string(m_directory + '/' + file.C_Str()), pTexType);
				texturesOut.push_back(tex);
				s_loadedTextures.push_back(tex);
			}
		}
		return texturesOut;
	}

	void Model::LoadTexturesToShader()
	{
		unsigned int diffuseNr = 0;
		unsigned int specularNr = 0;
		for (unsigned int i = 0; i < m_textures.size(); ++i)
		{
			// Retrieve texture number (the N in diffuse_textureN)
			string name;
			string number;
			switch (m_textures[i]->GetType())
			{
				case TexType::diffuse:
					name = "texture_diffuse";
					number = std::to_string(diffuseNr++);
					break;
				case TexType::specular:
					name = "texture_specular";
					number = std::to_string(specularNr++);
					break;
			}

			#ifdef _DEBUG
			 cout << " \xC5Loading texID " << m_textures[i]->GetId() << endl;
			#endif
			m_shaderRef->SetInt(("u_material." + name + number).c_str(), m_textures[i]->GetId());
		}
	}

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
}
