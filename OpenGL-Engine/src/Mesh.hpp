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
	// Static
	private:
		static float* s_cubeVerticesArr;
		static unsigned int* s_indicesArr;

	public:
		static vector<Vertex> GenerateVertices();
		static vector<unsigned int> GenerateIndices();

	// Members
	private:
		unique_ptr<vector<Vertex>> m_vertices = nullptr;
		unique_ptr<vector<unsigned int>> m_indices = nullptr;
        unique_ptr<vector<Texture*>> m_textures = nullptr;

		unsigned int* m_idVAO = new unsigned int(0U);	// The id for the vertex attribute object
		unsigned int* m_idVBO = new unsigned int(0U);	// The id for the vertex buffer object
		unsigned int* m_idEBO = new unsigned int(0U);	// The id for the element buffer object

		void SetupMesh();

	public:
		Mesh();
		Mesh(vector<Vertex> pVertices, vector<unsigned int> pIndices);
		Mesh(vector<Vertex> pVertices, vector<unsigned int> pIndices, vector<Texture*> pTextures);

		void Destroy(bool pValidate);

		// Legacy
		void LoadTexturesToShader(Shader& pShader);
		void Draw();

		#pragma region Getters
		unsigned int* GetVAO() const { return m_idVAO; }
		unsigned int* GetVBO() const { return m_idVBO; }
		unsigned int* GetEBO() const { return m_idEBO; }
		#pragma endregion
	};
}
