#include "Input.hpp"
#include "GLFW/glfw3.h"

#ifdef _DEBUG
 #include "Debug.hpp"
#endif

namespace Engine
{
	// Static

	void Input::Key_callback(GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods) noexcept
	{
		if (pAction != GLFW_PRESS) return;
		#ifdef _DEBUG
		 Engine::Debug::Send(std::to_string(pScancode) + " ", false, false);
		#endif
	}

	void Input::Mouse_callback(GLFWwindow* pWindow, double pPosX, double pPosY) noexcept
	{
		Input::GetInstance()->mouseCallback(pPosX, pPosY);
	}

	void Input::Scroll_callback(GLFWwindow* pWindow, double pOffsetX, double pOffsetY) noexcept
	{
		Input::GetInstance()->scrollCallback(pOffsetX, pOffsetY);
	}

	// Member

	void Input::Init(GLFWwindow* const& pWindowRef) noexcept
	{
		m_windowRef = pWindowRef;

		//glfwGetCursorPos(m_windowRef, &m_mouseLastX, &m_mouseLastY);
		glfwSetInputMode(m_windowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_windowRef, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			
		glfwSetKeyCallback(m_windowRef, Key_callback);
		glfwSetCursorPosCallback(m_windowRef, Mouse_callback);
		glfwSetScrollCallback(m_windowRef, Scroll_callback);
	}

	void Input::Process() noexcept
	{
		// TODO
	}

	bool Input::GetKey(Key pKey, State pState) noexcept
	{
		return (int)pState == glfwGetKey(m_windowRef, (int)pKey);
	}

	void Input::AddMouseCallback(CallbackFunc f) noexcept
	{
		mouseCallback = f;
	}

	void Input::AddSrollCallback(CallbackFunc f) noexcept
	{
		scrollCallback = f;
	}
}
