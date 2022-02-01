#pragma once
#include <vector>

using std::vector;

namespace Engine
{
	class Mesh
	{
	public:
		Mesh();
		Mesh(vector<float> pVertices, vector<unsigned int> pIndices);
		Mesh(vector<float>* pVertices, vector<unsigned int>* pIndices);
		~Mesh();

		// Copy constructors
		Mesh(const Mesh& pOther);
		Mesh(Mesh&& pOther);
		Mesh& operator=(const Mesh& pOther);
		Mesh& operator=(Mesh&& pOther);

		vector<float>* GetVertices() const;
		void SetVertices(vector<float> pVertices);
		vector<unsigned int>* GetIndices() const;
		void SetIndices(vector<unsigned int> pIndices);
		unsigned int* GetVAO();
		unsigned int* GetVBO();
		unsigned int* GetEBO();

	private:
		vector<float>* m_vertices = nullptr;
		vector<unsigned int>* m_indices = nullptr;

		unsigned int m_idVAO = 0U, m_idVBO = 0U, m_idEBO = 0U; // The vertex attribute object, vertex buffer object, and element buffer object

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
