#pragma once
#include <string>
#include <stdint.h>

struct GLFWwindow;

namespace srender
{
/** For outputting important text-based information to the console during program execution
 * @todo Make debug use imgui - Can open an "event" then post whatever info to it and finally close it
 * @todo Fix wcout not working some times for unknown reasons
 */
namespace debug
{
	enum class type: uint8_t
	{
		note = 0,
		process = 1
	};

	enum class impact: uint8_t
	{
		small = 0,
		large = 2
	};

	enum class stage: uint8_t
	{
		begin = 0,
		mid = 4,
		end = 8
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
}
}
