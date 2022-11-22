#pragma once
#include "Shader.hpp"
#include <memory>

namespace Engine
{
	struct Vertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
	};

	/** A mesh is a collection of vertices used to draw shapes */
	class Mesh
	{
		/** Hard coded vertices for a cube
		 * @deprecated No longer used since Model class
		 */
		static constexpr float s_cubeVerticesArr[288] = {
			// Positions				// Normals				  // Texture coords
		   -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
		    0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
		    0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		    0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
		   -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

		   -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
		    0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
		    0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
		    0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
		   -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

		   -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		   -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		   -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

		    0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
		    0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
		    0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		    0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
		    0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
		    0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

		   -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
		    0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
		    0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
		    0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
		   -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
		   -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

		   -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
		    0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
		    0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
		    0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
		   -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
		   -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
		};

		/** Hard coded indices for a cube
		 * @deprecated No longer used since Model class
		 */
		static constexpr uint32_t s_indicesArr[36] = {
			0U,	1U,	2U,	3U,	4U,	5U,	// Face 1
			6U, 	7U,	8U,	9U,	10U,	11U,	// Face 2
			12U,	13U,	14U,	15U,	16U,	17U,	// Face 3
			18U,	19U,	20U,	21U,	22U,	23U,	// Face 4
			24U,	25U,	26U,	27U,	28U,	29U,	// Face 5
			30U,	31U,	32U,	33U,	34U,	35U	// Face 6
		};

		uint32_t* m_idVAO = new uint32_t(0U);	// The id for the vertex attribute object
		uint32_t* m_idVBO = new uint32_t(0U);	// The id for the vertex buffer object
		uint32_t* m_idEBO = new uint32_t(0U);	// The id for the element buffer object

		std::unique_ptr<std::vector<Vertex>> m_vertices = nullptr;
		std::unique_ptr<std::vector<unsigned int>> m_indices = nullptr;

		void SetupMesh() const noexcept;

	public:
		_NODISCARD_MSG("Engine::Mesh::GenerateVertices()")
		/** Takes the vertices array and places it in a vector
		 * @deprecated No longer used since Model class
		 * @return [std::vector<Vertex>] The vertex vector
		 */
		static constexpr std::vector<Vertex> GenerateVertices() noexcept;
		_NODISCARD_MSG("Engine::Mesh::GenerateIndices()")
		/** Takes the indices array and places it in a vector
		 * @deprecated No longer used since Model class
		 * @return [std::vector<uint32_t>] The indices vector
		 */
		static constexpr std::vector<uint32_t> GenerateIndices() noexcept;

		Mesh();
		Mesh(
			const std::vector<Vertex>* pVertices,
			const std::vector<uint32_t>* pIndices);
		~Mesh();

		void Draw() const noexcept;

		_NODISCARD_MSG("Engine::Mesh::GetVAO()")
		/** Get the id for the vertex attribute object
		 * @return [uint32_t*] The id of the vertex attribute object
		 */
		uint32_t* GetVAO() const noexcept { return m_idVAO; }
		_NODISCARD_MSG("Engine::Mesh::GetVBO()")
		/** Get the id for the vertex buffer object
		 * @return [uint32_t*] The id of the vertex buffer object
		 */
		uint32_t* GetVBO() const noexcept { return m_idVBO; }
		_NODISCARD_MSG("Engine::Mesh::GetEBO()")
		/** Get the id for the element buffer object
		 * @return [uint32_t*] The id of the element buffer object
		 */
		uint32_t* GetEBO() const noexcept { return m_idEBO; }
	};
}
