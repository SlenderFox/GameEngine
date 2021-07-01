#pragma once
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"

class Application
{
public:
	Application();
	~Application();

	bool run();

private:
	void processInput(GLFWwindow* pWindow);

	const unsigned int winWidth, winHeight;
	unsigned int m_idVAO, m_idVBO, m_idEBO;

	// Creates 4 verts with each one having: xyz position, rgb colour and xy texcoord
	const float m_vertices[32] =
	{
		// Positions          // Colours          // Texture coords
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,   // Top left 
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f    // Bottom left
	};
	// How to construct the triangles using the verts
	const unsigned int m_indices[6]
	{
		0, 1, 2,    // Triangle one
		0, 2, 3     // Triangle two
	};

	GLFWwindow* m_window = nullptr;
	Texture* m_textureRef = nullptr;
	Shader* m_shaderRef = nullptr;
};