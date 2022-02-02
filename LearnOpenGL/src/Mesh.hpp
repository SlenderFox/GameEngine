#pragma region
#pragma once
#include <vector>
#include <memory>
#include "Shader.hpp"
#include "Texture.hpp"

using std::vector;
using std::unique_ptr;
using std::make_unique;
#pragma endregion

namespace Engine
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(vector<float> pVertices, vector<unsigned int> pIndices);
		Mesh(unique_ptr<vector<float>> pVertices, unique_ptr<vector<unsigned int>> pIndices);

		// Copy constructors
		Mesh(const Mesh& pOther);
		Mesh(Mesh&& pOther) noexcept;
		Mesh& operator=(const Mesh& pOther);
		Mesh& operator=(Mesh&& pOther) noexcept;

		void Destroy(bool pValidate);

		vector<float>* GetVertices() const;
		void SetVertices(vector<float>* pVertices);
		vector<unsigned int>* GetIndices() const;
		void SetIndices(vector<unsigned int>* pIndices);
		unsigned int* GetVAO() const;
		unsigned int* GetVBO() const;
		unsigned int* GetEBO() const;
		
	private:
		unique_ptr<vector<float>> m_vertices = nullptr;
		unique_ptr<vector<unsigned int>> m_indices = nullptr;

		unsigned int* m_idVAO = new unsigned int(0U);	// The id for the vertex attribute object
		unsigned int* m_idVBO = new unsigned int(0U);	// The id for the vertex buffer object
		unsigned int* m_idEBO = new unsigned int(0U);	// The id for the element buffer object

		// Creates 4 verts with each one having: xyz position, rgb colour and xy texcoord
		//const float m_vertices[32] =
		//{
		//	// Positions          // Colours          // Texture coords
		//	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Top left 
		//	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
		//	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
		//	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f    // Bottom left
		//};
		//
		// More basic cube
		//float vertices[] = {
		//    -0.5f, -0.5f, -0.5f,
		//     0.5f, -0.5f, -0.5f,
		//     0.5f,  0.5f, -0.5f,
		//     0.5f,  0.5f, -0.5f,
		//    -0.5f,  0.5f, -0.5f,
		//    -0.5f, -0.5f, -0.5f,
		//
		//    -0.5f, -0.5f,  0.5f,
		//     0.5f, -0.5f,  0.5f,
		//     0.5f,  0.5f,  0.5f,
		//     0.5f,  0.5f,  0.5f,
		//    -0.5f,  0.5f,  0.5f,
		//    -0.5f, -0.5f,  0.5f,
		//
		//    -0.5f,  0.5f,  0.5f,
		//    -0.5f,  0.5f, -0.5f,
		//    -0.5f, -0.5f, -0.5f,
		//    -0.5f, -0.5f, -0.5f,
		//    -0.5f, -0.5f,  0.5f,
		//    -0.5f,  0.5f,  0.5f,
		//
		//     0.5f,  0.5f,  0.5f,
		//     0.5f,  0.5f, -0.5f,
		//     0.5f, -0.5f, -0.5f,
		//     0.5f, -0.5f, -0.5f,
		//     0.5f, -0.5f,  0.5f,
		//     0.5f,  0.5f,  0.5f,
		//
		//    -0.5f, -0.5f, -0.5f,
		//     0.5f, -0.5f, -0.5f,
		//     0.5f, -0.5f,  0.5f,
		//     0.5f, -0.5f,  0.5f,
		//    -0.5f, -0.5f,  0.5f,
		//    -0.5f, -0.5f, -0.5f,
		//
		//    -0.5f,  0.5f, -0.5f,
		//     0.5f,  0.5f, -0.5f,
		//     0.5f,  0.5f,  0.5f,
		//     0.5f,  0.5f,  0.5f,
		//    -0.5f,  0.5f,  0.5f,
		//    -0.5f,  0.5f, -0.5f
		//};
		
		// Creates a cube, doesn't use EBO
		float* m_verticesArr = new float[288] {
			// Positions          // Colours          // Texture coords
			-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f
		};

		// How to construct the triangles using the verts
		unsigned int* m_indicesArr = new unsigned int[6] {
			0U, 1U, 2U,    // Triangle one
			0U, 2U, 3U     // Triangle two
		};
	};
}
