#pragma once
#include <glad/glad.h> // Include glad to get all the required OpenGL headers
#include <memory>
#include "GameObject.hpp"
#include "Mesh.hpp"

using std::vector;
using std::unique_ptr;

namespace Engine
{
	class Renderer
	{
		friend class Application;	// Allowing application access and control
	public:
		static Renderer* GetInstance()
		{
			static Renderer* sm_instance = new Renderer();
			return sm_instance;
		}

		void CreateVAO(unsigned int& pidVAO, unsigned int& pidVBO, unsigned int& pidEBO,
		 float* pVertices[], const size_t pVertexSize, unsigned int* pIndices[], const size_t pIndexSize);
		void CreateVAO(unsigned int& pidVAO, unsigned int& pidVBO, unsigned int& pidEBO,
		 std::vector<float>* pVertices, std::vector<unsigned int>* pIndices);

	private:
		Renderer() = default;
		~Renderer() {}
		// Delete copy/move so extra instances can't be created/moved.
		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;
		Renderer(Renderer&&) = delete;
		Renderer& operator=(Renderer&&) = delete;

		/**
		 * @brief Initlialises the renderer
		 *
		 * @return If initlialisation was successful
		 */
		bool Init();
		/**
		 * @brief Destroys the renderer but only if it was ever initliased
		 *
		 * @param pValidate Whether the renderer was even initialised
		 */
		void Destroy(bool pValidate);
		/**
		 * @brief Draws the scene
		 *
		 * @param pCamera TEMPORARY! The viewprojection matrix used as the camera
		 * @param pTime TEMPORARY! Used for basic shape animation
		 * @remark Only Application is able to call this function
		 */
		void Draw(glm::mat4 pCamera, double pTime);

		Shader* m_shaderRef = nullptr;      // A reference to a shader
		Texture* m_textureRef = nullptr;    // A reference to a texture
		vector<unique_ptr<Mesh>>* m_meshes = nullptr;

		//Mesh* test = nullptr;

		unsigned int m_idVAO = 0U, m_idVBO = 0U, m_idEBO = 0U; // The vertex attribute object, vertex buffer object, and element buffer object

		const glm::vec3 m_cubePositions[10] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
		};
		
		// // Creates 4 verts with each one having: xyz position, rgb colour and xy texcoord
		// const float m_vertices[32] =
		// {
		// 	// Positions          // Colours          // Texture coords
		// 	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Top left
		// 	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
		// 	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
		// 	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f    // Bottom left
		// };
		//
		// // More basic cube
		// float vertices[] = {
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
		// };
		//
		// 1152U 24U
		// // Creates a cube, doesn't use EBO
		// float* m_vertices = new float[288]
		// {
		// 	// Positions          // Colours          // Texture coords
		// 	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		// 	 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		//
		// 	-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		// 	 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		//
		// 	-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		// 	-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		//
		// 	 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		// 	 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		//
		// 	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	 0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	 0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	-0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		// 	-0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		//
		// 	-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f,
		// 	 0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 1.0f,
		// 	 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	 0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   1.0f, 0.0f,
		// 	-0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 0.0f,
		// 	-0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,   0.0f, 1.0f
		// };
		//
		// // How to construct the triangles using the verts
		// unsigned int* m_indices = new unsigned int[6]
		// {
		// 	0U, 1U, 2U,    // Triangle one
		// 	0U, 2U, 3U     // Triangle two
		// };
	};
}
