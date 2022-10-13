#ifdef _DEBUG
#include "Debug.hpp"
#include <iostream>
#include <io.h>
#include <fcntl.h>

namespace Engine
{
	void Debug::SendWithPrefix(const wchar_t* pPrefix, std::string& pMsg, bool pNewline, bool pEndline) noexcept
	{
		if (pNewline) std::wcout << '\n';
		std::wcout << pPrefix << pMsg.c_str();
		if (pEndline) std::wcout << std::endl;
	}

	void Debug::Init() noexcept
	{
		// Allows utf16 output
		_setmode(_fileno(stdout), _O_U16TEXT);
	}

	void Debug::Send(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		if (pNewline) std::wcout << '\n';
		std::wcout << pMsg.c_str();
		if (pEndline) std::wcout << std::endl;
	}

	void Debug::NewLine() noexcept
	{
		std::wcout << std::endl;
	}
	
	#pragma region Init
	void Debug::InitSmallNote(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ┌ \xDA 218 \u250C
		SendWithPrefix(L"\u250C", pMsg, pNewline, pEndline);
	}

	void Debug::InitBigNote(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ╒ \xD5 213 \u2552
		SendWithPrefix(L"\u2552", pMsg, pNewline, pEndline);
	}

	void Debug::InitSmallProcess(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ┬ \xC2 194 \u252C
		SendWithPrefix(L"\u252C", pMsg, pNewline, pEndline);
	}

	void Debug::InitBigProcess(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ╤ \xD1 209 \u2564
		SendWithPrefix(L"\u2564", pMsg, pNewline, pEndline);
	}
	#pragma endregion
	#pragma region Mid
	void Debug::SmallNote(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ├ \xC3 195 \u251C
		SendWithPrefix(L"\u251C", pMsg, pNewline, pEndline);
	}

	void Debug::BigNote(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ╞ \xC6 198 \u255E
		SendWithPrefix(L"\u255E", pMsg, pNewline, pEndline);
	}

	void Debug::SmallProcess(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ┼ \xC5 197 \u253C
		SendWithPrefix(L"\u253C", pMsg, pNewline, pEndline);
	}

	void Debug::BigProcess(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ╪ \xD8 216 \u256A
		SendWithPrefix(L"\u256A", pMsg, pNewline, pEndline);
	}
	#pragma endregion
	#pragma region End
	void Debug::EndSmallNote(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// └ \xC0 192 \u2514
		SendWithPrefix(L"\u2514", pMsg, pNewline, pEndline);
	}

	void Debug::EndBigNote(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ╘ \xD4 212 \u2558
		SendWithPrefix(L"\u2558", pMsg, pNewline, pEndline);
	}

	void Debug::EndSmallProcess(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ┴ \xC1 193 \u2534
		SendWithPrefix(L"\u2534", pMsg, pNewline, pEndline);
	}

	void Debug::EndBigProcess(std::string pMsg, bool pNewline, bool pEndline) noexcept
	{
		// ╧ \xCF 207 \u2567
		SendWithPrefix(L"\u2567", pMsg, pNewline, pEndline);
	}
	#pragma endregion
}
#endif
