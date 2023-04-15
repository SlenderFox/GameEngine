#pragma once
#include <string>

struct GLFWwindow;

namespace srender
{
	/** For outputting important text-based information to the console during program execution
	 * @todo Make Debug use imgui - Can open an "event" then post whatever info to it and finally close it
	 */
	namespace debug
	{
		enum class type: uint8_t
		{
			Note = 0,
			Process = 1
		};

		enum class impact: uint8_t
		{
			Small = 0,
			Large = 2
		};

		enum class stage: uint8_t
		{
			Begin = 0,
			Mid = 4,
			End = 8
		};

		void init() noexcept;

		void send(
			const std::string _msg,
			const bool _newline = false,
			const bool _endline = true
		) noexcept;
		void send(
			const std::string _msg,
			const wchar_t *_prefix,
			const bool _newline = false,
			const bool _endline = true
		) noexcept;
		void send(
			const std::string _msg,
			const type _type,
			const impact _impact,
			const stage _stage,
			const bool _newline = false,
			const bool _endline = true
		) noexcept;

		void newLine() noexcept;

		#if defined(_WIN32) && defined(_DEBUG)
			void moveConsole(const uint16_t _width) noexcept;
		#endif
	};
}
