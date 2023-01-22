#include "input.hpp"
#include "GLFW/glfw3.h"
#include "debug.hpp"

namespace srender
{
	GLFWwindow *input::s_windowRef = nullptr;
	callbackFunc input::s_mouseCallbackFun = nullptr,
		input::s_scrollCallbackFun = nullptr;
	double input::s_mouseX = 0.0,
		input::s_mouseY = 0.0,
		input::s_mouseLastX = 0.0,
		input::s_mouseLastY = 0.0;

	void input::key_callback(
		GLFWwindow *pWindow,
		int pKey,
		int pScancode,
		int pAction,
		int pMods) noexcept
	{
		if (pAction != GLFW_PRESS) return;
		srender::debug::send(std::to_string(pScancode) + " ", false, false);
	}

	void input::mouse_callback(
		GLFWwindow *pWindow,
		double pPosX,
		double pPosY) noexcept
	{
		s_mouseLastX = s_mouseX;
		s_mouseLastY = s_mouseY;
		s_mouseX = pPosX;
		s_mouseY = pPosY;
		double deltaX = s_mouseX - s_mouseLastX;
		double deltaY = s_mouseY - s_mouseLastY;
		if (s_mouseCallbackFun)
			s_mouseCallbackFun(deltaX, deltaY);
	}

	void input::scroll_callback(
		GLFWwindow *pWindow,
		double pOffsetX,
		double pOffsetY) noexcept
	{
		if (s_scrollCallbackFun)
			s_scrollCallbackFun(pOffsetX, pOffsetY);
	}

	bool input::init(GLFWwindow *pWindowRef) noexcept
	{
		s_windowRef = pWindowRef;

		glfwGetCursorPos(s_windowRef, &s_mouseX, &s_mouseY);
		s_mouseLastX = s_mouseX;
		s_mouseLastY = s_mouseY;

		glfwSetInputMode(s_windowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(s_windowRef, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSetKeyCallback(s_windowRef, key_callback);
		glfwSetCursorPosCallback(s_windowRef, mouse_callback);
		glfwSetScrollCallback(s_windowRef, scroll_callback);

		return true;
	}

	void input::process() noexcept
	{
		// TODO
	}

	bool input::checkKeyState(const key pKey, const state pState) noexcept
	{
		return (int)pState == glfwGetKey(s_windowRef, (int)pKey);
	}
}
