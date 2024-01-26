#include "debug.hpp"
#include "winclude.hpp"
#ifdef _WIN32
	#include <io.h>
	#include <fcntl.h>
#endif
#include <iostream>

using std::wcout;
using std::endl;
using std::string;

namespace srender
{
namespace debug
{
	/** List of prefixes:
	 * @note 00: begin-small-note   : ┌ \xDA 218 \u250C
	 * @note 01: begin-small-process: ┬ \xC2 194 \u252C
	 * @note 02: begin-large-note   : ╒ \xD5 213 \u2552
	 * @note 03: begin-large-process: ╤ \xD1 209 \u2564
	 * @note 04:   mid-small-note   : ├ \xC3 195 \u251C
	 * @note 05:   mid-small-process: ┼ \xC5 197 \u253C
	 * @note 06:   mid-large-note   : ╞ \xC6 198 \u255E
	 * @note 07:   mid-large-process: ╪ \xD8 216 \u256A
	 * @note 08:   end-small-note   : └ \xC0 192 \u2514
	 * @note 09:   end-small-process: ┴ \xC1 193 \u2534
	 * @note 10:   end-large-note   : ╘ \xD4 212 \u2558
	 * @note 11:   end-large-process: ╧ \xCF 207 \u2567
	 */
	const wchar_t *l_prefixes[12] = {
		L"\u250C",
		L"\u252C",
		L"\u2552",
		L"\u2564",
		L"\u251C",
		L"\u253C",
		L"\u255E",
		L"\u256A",
		L"\u2514",
		L"\u2534",
		L"\u2558",
		L"\u2567",
	};

	void init() noexcept
	{
		#if defined(_WIN32) && defined(_DEBUG)
			// Allows utf16 output to console
			_setmode(_fileno(stdout), _O_U16TEXT);
		#endif
	}

	void send(
		const string _msg,
		const bool _newline,
		const bool _endline
	) noexcept
	{
		#ifdef _DEBUG
			if (_newline)
			{	wcout << '\n'; }
			wcout << _msg.c_str();
			if (_endline)
			{	wcout << endl; }
		#endif
	}

	void send(
		const string _msg,
		const wchar_t *_prefix,
		const bool _newline,
		const bool _endline
	) noexcept
	{
		#ifdef _DEBUG
			if (_newline)
			{	wcout << '\n'; }
			wcout << _prefix << _msg.c_str();
			if (_endline)
			{	wcout << endl; }
		#endif
	}

	void send(
		const string _msg,
		const type _type,
		const impact _impact,
		const stage _stage,
		const bool _newline,
		const bool _endline
	) noexcept
	{
		#ifdef _DEBUG
			uint8_t prefix = (uint8_t)_type + (uint8_t)_impact + (uint8_t)_stage;
			send(_msg, l_prefixes[prefix], _newline, _endline);
		#endif
	}

	void newLine() noexcept
	{
		#ifdef _DEBUG
			wcout << endl;
		#endif
	}

	#if defined(_WIN32) && defined(_DEBUG)
		void moveConsole(const uint16_t _width) noexcept
		{
			// Moves the console to the right and resizes
			MoveWindow(GetConsoleWindow(), _width - 3, 0, 900, 1040, TRUE);
		}
	#endif
}
}
