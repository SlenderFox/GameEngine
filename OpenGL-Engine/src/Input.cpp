#pragma region
#include "Input.hpp"
#include "GLFW/glfw3.h"

#ifdef _DEBUG
 #include "Debug.hpp"
#endif
#pragma endregion

namespace Engine
{
	// Static

	bool Input::s_KeyCallback = true;
	bool Input::s_MouseCallback = false;
	bool Input::s_ScrollCallback = false;

	void Input::Key_callback(GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods) noexcept
	{
		if (!s_KeyCallback) return;
		if (pAction != GLFW_PRESS) return;
#		ifdef _DEBUG
		 Engine::Debug::Send(std::to_string(pScancode) + " ", false, false);
#		endif
	}

	void Input::Mouse_callback(GLFWwindow* pWindow, double pPosX, double pPosY) noexcept
	{
		if (!s_MouseCallback) return;
		Input::GetInstance()->mouseCallback(pPosX, pPosY);
	}

	void Input::Scroll_callback(GLFWwindow* pWindow, double pOffsetX, double pOffsetY) noexcept
	{
		if (!s_ScrollCallback) return;
		Input::GetInstance()->scrollCallback(pOffsetX, pOffsetY);
	}

	// Member

	bool Input::Init(GLFWwindow* const& pWindowRef) noexcept
	{
		m_windowRef = pWindowRef;

		//glfwGetCursorPos(m_windowRef, &m_mouseLastX, &m_mouseLastY);
		//glfwSetInputMode(m_windowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_windowRef, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			
		glfwSetKeyCallback(m_windowRef, Key_callback);
		glfwSetCursorPosCallback(m_windowRef, Mouse_callback);
		glfwSetScrollCallback(m_windowRef, Scroll_callback);

		return true;
	}

	void Input::Process() noexcept
	{
		// TODO
	}

	bool Input::GetKey(const Key& pKey, const State& pState) noexcept
	{
		return (int)pState == glfwGetKey(m_windowRef, (int)pKey);
	}

	void Input::AddMouseCallback(CallbackFunc f) noexcept
	{
		mouseCallback = f;
		s_MouseCallback = true;
	}

	void Input::AddSrollCallback(CallbackFunc f) noexcept
	{
		scrollCallback = f;
		s_ScrollCallback = true;
	}
}
