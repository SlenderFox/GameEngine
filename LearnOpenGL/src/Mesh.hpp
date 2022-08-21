#pragma region
#pragma once
#include <vector>
#include <memory>
#include "Shader.hpp"

using std::vector;
using std::unique_ptr;
using std::make_unique;
#pragma endregion

namespace Engine
{
	struct Vertex {
		vec3 position;
		vec3 normal;
		vec2 texCoords;
	};

	class Mesh
	{
	public:
		Mesh(vector<Vertex> pVertices, vector<unsigned int> pIndices, vector<Texture> pTextures = vector<Texture>());
		Mesh(unique_ptr<vector<Vertex>> pVertices, unique_ptr<vector<unsigned int>> pIndices, unique_ptr<vector<Texture>> pTextures = make_unique<vector<Texture>>());
		Mesh();

		#pragma region Copy constructors
		Mesh(const Mesh& pOther);
		Mesh(Mesh&& pOther) noexcept;
		Mesh& operator=(const Mesh& pOther);
		Mesh& operator=(Mesh&& pOther) noexcept;
		#pragma endregion

		void Destroy(bool pValidate);

		void LoadTextures(Shader& pShader);
		void Draw(Shader* pShader);

		static vector<Vertex> GenerateVertices();
		static vector<unsigned int> GenerateIndices();

		#pragma region Setters
		void SetVertices(vector<Vertex>* pVertices);
		void SetIndices(vector<unsigned int>* pIndices);
		void SetTextures(vector<Texture>* pTextures);
		#pragma endregion
		#pragma region Getters
		vector<Vertex>* GetVertices() const;
		vector<unsigned int>* GetIndices() const;
		vector<Texture>* GetTextures() const;
		unsigned int* GetVAO() const;
		unsigned int* GetVBO() const;
		unsigned int* GetEBO() const;
		#pragma endregion
		
	private:
		static float* s_cubeVerticesArr;
		static unsigned int* s_indicesArr;

		void SetupMesh();

		unique_ptr<vector<Vertex>> m_vertices = nullptr;
		unique_ptr<vector<unsigned int>> m_indices = nullptr;
        unique_ptr<vector<Texture>> m_textures = nullptr;

		unsigned int* m_idVAO = new unsigned int(0U);	// The id for the vertex attribute object
		unsigned int* m_idVBO = new unsigned int(0U);	// The id for the vertex buffer object
		unsigned int* m_idEBO = new unsigned int(0U);	// The id for the element buffer object
	};
}
