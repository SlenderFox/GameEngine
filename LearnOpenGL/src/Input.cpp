#include "Input.h"

Input::Input()
{
}

Input::~Input()
{
}

void Input::ProcessInput(GLFWwindow* pWindow)
{
    if (glfwGetKey(pWindow, GLFW_KEY_END) == GLFW_PRESS)
        glfwSetWindowShouldClose(pWindow, true);

    if (glfwGetKey(pWindow, GLFW_KEY_F1) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (glfwGetKey(pWindow, GLFW_KEY_F2) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}
