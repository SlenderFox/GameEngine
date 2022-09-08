#pragma region
#pragma once
#include "Shader.hpp"
#include <vector>
#include <memory>

#pragma endregion

namespace Engine
{
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	class Mesh
	{
	// Static
	private:
		static float* s_cubeVerticesArr;
		static unsigned int* s_indicesArr;

	public:
		static std::vector<Vertex> GenerateVertices();
		static std::vector<unsigned int> GenerateIndices();

	// Members
	private:
		std::unique_ptr<std::vector<Vertex>> m_vertices = nullptr;
		std::unique_ptr<std::vector<unsigned int>> m_indices = nullptr;
        std::unique_ptr<std::vector<Texture*>> m_textures = nullptr;

		unsigned int* m_idVAO = new unsigned int(0U);	// The id for the vertex attribute object
		unsigned int* m_idVBO = new unsigned int(0U);	// The id for the vertex buffer object
		unsigned int* m_idEBO = new unsigned int(0U);	// The id for the element buffer object

		void SetupMesh();

	public:
		Mesh();
		Mesh(std::vector<Vertex> pVertices, std::vector<unsigned int> pIndices);
		Mesh(std::vector<Vertex> pVertices, std::vector<unsigned int> pIndices, std::vector<Texture*> pTextures);

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
