#include "Input.hpp"
#include "GLFW/glfw3.h"

#ifdef _DEBUG
 #include "Debug.hpp"
#endif

namespace Engine
{
	void Input::Key_callback(GLFWwindow* pWindow, int pKey, int pScancode, int pAction, int pMods)
	{
		if (pAction != GLFW_PRESS) return;
		#ifdef _DEBUG
		 Engine::Debug::Send(std::to_string(pScancode) + " ", false, false);
		#endif
	}

	void Input::Init(GLFWwindow* const& pWindowRef) noexcept
	{
		m_windowRef = pWindowRef;

		glfwSetInputMode(m_windowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_windowRef, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	void Input::Process() noexcept
	{
		// TODO
	}

	bool Input::GetKey(Key pKey, State pState) noexcept
	{
		return (int)pState == glfwGetKey(m_windowRef, (int)pKey);
	}
}
