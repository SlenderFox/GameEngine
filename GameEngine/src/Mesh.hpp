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
		static const float* s_cubeVerticesArr;
		static const uint32_t* s_indicesArr;
		// ↑↓ Both of these are depreciated
	public:
		static std::vector<Vertex> GenerateVertices() noexcept;
		static std::vector<uint32_t> GenerateIndices() noexcept;

	private:
		std::unique_ptr<std::vector<Vertex>> m_vertices = nullptr;
		std::unique_ptr<std::vector<unsigned int>> m_indices = nullptr;

		uint32_t* m_idVAO = new uint32_t(0U);	// The id for the vertex attribute object
		uint32_t* m_idVBO = new uint32_t(0U);	// The id for the vertex buffer object
		uint32_t* m_idEBO = new uint32_t(0U);	// The id for the element buffer object

		void SetupMesh() const noexcept;

	public:
		Mesh();
		Mesh(
			const std::vector<Vertex>& pVertices,
			const std::vector<uint32_t>& pIndices);
		~Mesh();

		void Draw() const noexcept;

		constexpr uint32_t* GetVAO() const noexcept { return m_idVAO; }
		constexpr uint32_t* GetVBO() const noexcept { return m_idVBO; }
		constexpr uint32_t* GetEBO() const noexcept { return m_idEBO; }
	};
}
