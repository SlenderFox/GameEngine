#pragma once
#include <GLFW/glfw3.h>

class Application
{
public:
	Application();
	~Application();

	bool run();

private:
	void processInput(GLFWwindow* pWindow);

	const unsigned int winWidth = 800;
	const unsigned int winHeight = 600;
};