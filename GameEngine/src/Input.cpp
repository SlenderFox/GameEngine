#include "Input.hpp"
#include "GLFW/glfw3.h"
#include "Debug.hpp"

namespace Engine
{
#	pragma region Variables
	GLFWwindow* Input::s_windowRef = nullptr;
	CallbackFunc Input::s_mouseCallbackFun = nullptr,
		Input::s_scrollCallbackFun = nullptr;
	double Input::s_mouseX = 0.0,
		Input::s_mouseY = 0.0,
		Input::s_mouseLastX = 0.0,
		Input::s_mouseLastY = 0.0;
#	pragma endregion

#	pragma region Bullshit
	void Input::Key_callback(
		GLFWwindow* pWindow,
		int pKey,
		int pScancode,
		int pAction,
		int pMods) noexcept
	{
		if (pAction != GLFW_PRESS) return;
		Engine::Debug::Send(std::to_string(pScancode) + " ", false, false);
	}

	void Input::Mouse_callback(
		GLFWwindow* pWindow,
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

	void Input::Scroll_callback(
		GLFWwindow* pWindow,
		double pOffsetX,
		double pOffsetY) noexcept
	{
		if (s_scrollCallbackFun)
			s_scrollCallbackFun(pOffsetX, pOffsetY);
	}
#	pragma endregion

	bool Input::Init(GLFWwindow* pWindowRef) noexcept
	{
		s_windowRef = pWindowRef;

		glfwGetCursorPos(s_windowRef, &s_mouseX, &s_mouseY);
		s_mouseLastX = s_mouseX;
		s_mouseLastY = s_mouseY;

		glfwSetInputMode(s_windowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(s_windowRef, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSetKeyCallback(s_windowRef, Key_callback);
		glfwSetCursorPosCallback(s_windowRef, Mouse_callback);
		glfwSetScrollCallback(s_windowRef, Scroll_callback);

		return true;
	}

	void Input::Process() noexcept
	{
		// TODO
	}

	bool Input::GetKeyState(const Key pKey, const State pState) noexcept
	{
		return (int)pState == glfwGetKey(s_windowRef, (int)pKey);
	}
}
