#pragma once

struct GLFWwindow;

namespace srender
{
	/** The callback function used for mouse and scroll movement */
	using callbackFunc = void(*)(double, double);

	/** Pure static class for handling inputs
	 * @todo Overhaul
	 * @todo Track keystates
	 * @todo Key press callbacks
	*/
	class input
	{
		friend class application;
		// Does not need to be memory managed
		static GLFWwindow *s_windowRef;

		static callbackFunc s_mouseCallbackFun;
		static callbackFunc s_scrollCallbackFun;

		static double s_mouseX, s_mouseY, s_mouseLastX, s_mouseLastY;

		// Pure static class
		input() = delete;
		~input() = delete;
		// Delete copy/move so extra instances can't be created/moved.
		input(input const&) = delete;
		input &operator=(input const&) = delete;
		input(input&&) = delete;
		input &operator=(input&&) = delete;

	public:
		enum class state: unsigned char
		{
			Release,
			Press,
			Repeat
		};

		enum class key: short
		{
			Key_Unknown					= -1,
			Key_Space					= 32,
			Key_Apostrophe				= 39,
			Key_Comma					= 44,
			Key_Minus					= 45,
			Key_FullStop				= 46,
			Key_ForwardSlash			= 47,
			Key_0							= 48,
			Key_1							= 49,
			Key_2							= 50,
			Key_3							= 51,
			Key_4							= 52,
			Key_5							= 53,
			Key_6							= 54,
			Key_7							= 55,
			Key_8							= 56,
			Key_9							= 57,
			Key_SemiColon				= 59,
			Key_Equals					= 61,
			Key_A							= 65,
			Key_B							= 66,
			Key_C							= 67,
			Key_D							= 68,
			Key_E							= 69,
			Key_F							= 70,
			Key_G							= 71,
			Key_H							= 72,
			Key_I							= 73,
			Key_J							= 74,
			Key_K							= 75,
			Key_L							= 76,
			Key_M							= 77,
			Key_N							= 78,
			Key_O							= 79,
			Key_P							= 80,
			Key_Q							= 81,
			Key_R							= 82,
			Key_S							= 83,
			Key_T							= 84,
			Key_U							= 85,
			Key_V							= 86,
			Key_W							= 87,
			Key_X							= 88,
			Key_Y							= 89,
			Key_Z							= 90,
			Key_LeftSquareBracket	= 91,
			Key_BackSlash				= 92,
			Key_RightSquareBracket	= 93,
			Key_BackTick				= 96,
			Key_Escape					= 256,
			Key_Enter					= 257,
			Key_Tab						= 258,
			Key_Backspace				= 259,
			Key_Insert					= 260,
			Key_Delete					= 261,
			Key_Right					= 262,
			Key_Left						= 263,
			Key_Down						= 264,
			Key_Up						= 265,
			Key_PageUp					= 266,
			Key_PageDown				= 267,
			Key_Home						= 268,
			Key_End						= 269,
			Key_CapsLock				= 280,
			Key_ScrollLock				= 281,
			Key_NumLock					= 282,
			Key_PrintScreen			= 283,
			Key_Pause					= 284,
			Key_F1						= 290,
			Key_F2						= 291,
			Key_F3						= 292,
			Key_F4						= 293,
			Key_F5						= 294,
			Key_F6						= 295,
			Key_F7						= 296,
			Key_F8						= 297,
			Key_F9						= 298,
			Key_F10						= 299,
			Key_F11						= 300,
			Key_F12						= 301,
			Key_F13						= 302,
			Key_F14						= 303,
			Key_F15						= 304,
			Key_F16						= 305,
			Key_F17						= 306,
			Key_F18						= 307,
			Key_F19						= 308,
			Key_F20						= 309,
			Key_F21						= 310,
			Key_F22						= 311,
			Key_F23						= 312,
			Key_F24						= 313,
			Key_F25						= 314,
			Key_Keypad_0				= 320,
			Key_Keypad_1				= 321,
			Key_Keypad_2				= 322,
			Key_Keypad_3				= 323,
			Key_Keypad_4				= 324,
			Key_Keypad_5				= 325,
			Key_Keypad_6				= 326,
			Key_Keypad_7				= 327,
			Key_Keypad_8				= 328,
			Key_Keypad_9				= 329,
			Key_Keypad_Decimal		= 330,
			Key_Keypad_Divide			= 331,
			Key_Keypad_Multiply		= 332,
			Key_Keypad_Subtract		= 333,
			Key_Keypad_Add				= 334,
			Key_Keypad_Enter			= 335,
			Key_Keypad_Equals			= 336,
			Key_LeftShift				= 340,
			Key_LeftControl			= 341,
			Key_LeftAlt					= 342,
			Key_LeftSuper				= 343,
			Key_RightShift				= 344,
			Key_RightControl			= 345,
			Key_RightAlt				= 346,
			Key_RightSuper				= 347,
			Key_Menu						= 348
		};

		static void key_callback(
			GLFWwindow *_window,
			int _key,
			int _scancode,
			int _action,
			int _mods) noexcept;
		static void mouse_callback(
			GLFWwindow *_window,
			double _posX,
			double _posY) noexcept;
		static void scroll_callback(
			GLFWwindow *_window,
			double _offsetX,
			double _offsetY) noexcept;

		/** Initialise input with GLFW
		 * @param _windowRef The active window
		 * @return [bool] Success
		 */
		static bool init(GLFWwindow *_windowRef) noexcept;
		static void process() noexcept;
		static bool checkKeyState(const key _key, const state _state) noexcept;

		static inline void addMouseCallback(callbackFunc _callback) noexcept
		{	s_mouseCallbackFun = _callback; }
		static inline void addSrollCallback(callbackFunc _callback) noexcept
		{	s_scrollCallbackFun = _callback; }
	};
}
