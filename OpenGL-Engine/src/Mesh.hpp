#pragma once
#include "Shader.hpp"
#include <vector>
#include <memory>

namespace Engine
{
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	class Mesh
	{
	private:
		static float* s_cubeVerticesArr;
		static uint32_t* s_indicesArr;

	public:
		static std::vector<Vertex> GenerateVertices();
		static std::vector<uint32_t> GenerateIndices();

	private:
		std::unique_ptr<std::vector<Vertex>> m_vertices = nullptr;
		std::unique_ptr<std::vector<unsigned int>> m_indices = nullptr;

		uint32_t* m_idVAO = new uint32_t(0U);	// The id for the vertex attribute object
		uint32_t* m_idVBO = new uint32_t(0U);	// The id for the vertex buffer object
		uint32_t* m_idEBO = new uint32_t(0U);	// The id for the element buffer object

		void SetupMesh();

	public:
		Mesh();
		Mesh(std::vector<Vertex> pVertices, std::vector<uint32_t> pIndices);

		void Destroy(bool pValidate);
		void Draw();

		#pragma region Getters
		uint32_t* GetVAO() const { return m_idVAO; }
		uint32_t* GetVBO() const { return m_idVBO; }
		uint32_t* GetEBO() const { return m_idEBO; }
		#pragma endregion
	};
}
