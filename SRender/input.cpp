#include "input.hpp"
#include "GLFW/glfw3.h"
#include "debug.hpp"

namespace srender
{
namespace input
{
	// Does not need to be memory managed
	GLFWwindow *s_windowRef = nullptr;
	callbackFunc s_mouseCallbackFun = nullptr,
		s_scrollCallbackFun = nullptr;
	double s_mouseX = 0.0,
		s_mouseY = 0.0,
		s_mouseLastX = 0.0,
		s_mouseLastY = 0.0;

	void key_callback(
		GLFWwindow *_window,
		int _key,
		int _scancode,
		int _action,
		int _mods
	) noexcept
	{
		if (_action != GLFW_PRESS)
		{	return; }

		//debug::send(std::to_string(_scancode) + " ", false, false);
	}

	void mouse_callback(
		GLFWwindow *_window,
		double _posX,
		double _posY
	) noexcept
	{
		s_mouseLastX = s_mouseX;
		s_mouseLastY = s_mouseY;
		s_mouseX = _posX;
		s_mouseY = _posY;
		double deltaX = s_mouseX - s_mouseLastX;
		double deltaY = s_mouseY - s_mouseLastY;

		if (s_mouseCallbackFun)
		{	s_mouseCallbackFun(deltaX, deltaY); }
	}

	void scroll_callback(
		GLFWwindow *_window,
		double _offsetX,
		double _offsetY
	) noexcept
	{
		if (s_scrollCallbackFun)
		{	s_scrollCallbackFun(_offsetX, _offsetY); }
	}

	bool init(GLFWwindow *_windowRef) noexcept
	{
		s_windowRef = _windowRef;

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

	void process() noexcept
	{
		// TODO
	}

	bool checkKeyState(const key _key, const state _state) noexcept
	{	return (int)_state == glfwGetKey(s_windowRef, (int)_key); }

	void addMouseCallback(callbackFunc _callback) noexcept
	{	s_mouseCallbackFun = _callback; }

	void addSrollCallback(callbackFunc _callback) noexcept
	{	s_scrollCallbackFun = _callback; }
}
}
