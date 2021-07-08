#pragma once
#include <GLFW/glfw3.h>

class Input
{
public:
	Input() {}
	~Input() {}

	void ProcessInput(GLFWwindow* pWindow);
};