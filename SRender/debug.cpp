#include "debug.hpp"

//#ifdef _DEBUG
	#include "winclude.hpp"
	#ifdef __WINDOWS__
		#include <io.h>
		#include <fcntl.h>
	#endif
	#include <iostream>
	using std::wcout;
	using std::endl;
//#endif

using std::string;

namespace srender
{
	namespace debug
	{
		/** List of prefixes:
		 * @note 00: Begin-Small-Note   : ┌ \xDA 218 \u250C
		 * @note 01: Begin-Small-Process: ┬ \xC2 194 \u252C
		 * @note 02: Begin-Large-Note   : ╒ \xD5 213 \u2552
		 * @note 03: Begin-Large-Process: ╤ \xD1 209 \u2564
		 * @note 04:   Mid-Small-Note   : ├ \xC3 195 \u251C
		 * @note 05:   Mid-Small-Process: ┼ \xC5 197 \u253C
		 * @note 06:   Mid-Large-Note   : ╞ \xC6 198 \u255E
		 * @note 07:   Mid-Large-Process: ╪ \xD8 216 \u256A
		 * @note 08:   End-Small-Note   : └ \xC0 192 \u2514
		 * @note 09:   End-Small-Process: ┴ \xC1 193 \u2534
		 * @note 10:   End-Large-Note   : ╘ \xD4 212 \u2558
		 * @note 11:   End-Large-Process: ╧ \xCF 207 \u2567
		 */
		const wchar_t *prefixes[12] = {
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
			#if defined(_DEBUG) && defined(__WINDOWS__)
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
			//#ifdef _DEBUG
				if (_newline) wcout << '\n';
				wcout << _msg.c_str();
				if (_endline) wcout << endl;
			//#endif
		}

		void send(
			const string _msg,
			const wchar_t *_prefix,
			const bool _newline,
			const bool _endline
		) noexcept
		{
			//#ifdef _DEBUG
				if (_newline) wcout << '\n';
				wcout << _prefix << _msg.c_str();
				if (_endline) wcout << endl;
			//#endif
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
			uint8_t pre = (uint8_t)_type + (uint8_t)_impact + (uint8_t)_stage;
			send(_msg, prefixes[pre], _newline, _endline);
		}

		void newLine() noexcept
		{
			#ifdef _DEBUG
				wcout << endl;
			#endif
		}

		#ifdef _DEBUG
			void moveConsole(const uint16_t _width) noexcept
			{
				#ifdef __WINDOWS__
					// Moves the console to the right and resizes
					MoveWindow(GetConsoleWindow(), _width - 3, 0, 900, 1040, TRUE);
				#endif
			}
		#endif
	}
}
