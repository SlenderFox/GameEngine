#include "input.hpp"
#include "GLFW/glfw3.h"
#include "debug.hpp"

namespace srender
{
namespace input
{
	// Does not need to be memory managed
	GLFWwindow *l_windowRef = nullptr;
	callbackFunc l_mouseCallbackFun = nullptr,
		l_scrollCallbackFun = nullptr;
	double l_mouseX = 0.0,
		l_mouseY = 0.0,
		l_mouseLastX = 0.0,
		l_mouseLastY = 0.0;

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
		l_mouseLastX = l_mouseX;
		l_mouseLastY = l_mouseY;
		l_mouseX = _posX;
		l_mouseY = _posY;
		double deltaX = l_mouseX - l_mouseLastX;
		double deltaY = l_mouseY - l_mouseLastY;

		if (l_mouseCallbackFun)
		{	l_mouseCallbackFun(deltaX, deltaY); }
	}

	void scroll_callback(
		GLFWwindow *_window,
		double _offsetX,
		double _offsetY
	) noexcept
	{
		if (l_scrollCallbackFun)
		{	l_scrollCallbackFun(_offsetX, _offsetY); }
	}

	bool init(GLFWwindow *_windowRef) noexcept
	{
		l_windowRef = _windowRef;

		glfwGetCursorPos(l_windowRef, &l_mouseX, &l_mouseY);
		l_mouseLastX = l_mouseX;
		l_mouseLastY = l_mouseY;

		glfwSetInputMode(l_windowRef, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(l_windowRef, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSetKeyCallback(l_windowRef, key_callback);
		glfwSetCursorPosCallback(l_windowRef, mouse_callback);
		glfwSetScrollCallback(l_windowRef, scroll_callback);

		return true;
	}

	void process() noexcept
	{
		// TODO
	}

	bool checkKeyState(const key _key, const state _state) noexcept
	{	return (int)_state == glfwGetKey(l_windowRef, (int)_key); }

	void addMouseCallback(callbackFunc _callback) noexcept
	{	l_mouseCallbackFun = _callback; }

	void addSrollCallback(callbackFunc _callback) noexcept
	{	l_scrollCallbackFun = _callback; }
}
}
