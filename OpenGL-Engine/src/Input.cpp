#include "Input.hpp"
#include "GLFW/glfw3.h"

namespace Engine
{
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

	bool Input::GetKey(Code pKey, State pState) noexcept
	{
		return (int)pState == glfwGetKey(m_windowRef, (int)pKey);
	}
}
