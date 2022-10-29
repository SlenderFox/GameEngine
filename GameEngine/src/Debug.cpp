#pragma region
#include "Debug.hpp"
#include <io.h>
#include <fcntl.h>

#ifdef _DEBUG
# include <iostream>
 using std::wcout;
 using std::endl;
#endif

using std::string;
#pragma endregion

namespace Engine
{
	void Debug::Init() noexcept
	{
		// Allows utf16 output
		_setmode(_fileno(stdout), _O_U16TEXT);
	}

	void Debug::Send(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
#		ifdef _DEBUG
			if (pNewline) wcout << '\n';
			wcout << pMsg.c_str();
			if (pEndline) wcout << endl;
#		endif
	}

	void Debug::SendWithPrefix(
		const wchar_t* pPrefix, 
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
#		ifdef _DEBUG
			if (pNewline) wcout << '\n';
			wcout << pPrefix << pMsg.c_str();
			if (pEndline) wcout << endl;
#		endif
	}

	void Debug::NewLine() noexcept
	{
#		ifdef _DEBUG
			wcout << endl;
#		endif
	}
	
#	pragma region Start
	void Debug::NoteSmallStart(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ┌ \xDA 218 \u250C
		SendWithPrefix(L"\u250C", pMsg, pNewline, pEndline);
	}

	void Debug::NoteBigStart(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ╒ \xD5 213 \u2552
		SendWithPrefix(L"\u2552", pMsg, pNewline, pEndline);
	}

	void Debug::ProcessSmallStart(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ┬ \xC2 194 \u252C
		SendWithPrefix(L"\u252C", pMsg, pNewline, pEndline);
	}

	void Debug::ProcessBigStart(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ╤ \xD1 209 \u2564
		SendWithPrefix(L"\u2564", pMsg, pNewline, pEndline);
	}
#	pragma endregion

#	pragma region Mid
	void Debug::NoteSmall(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ├ \xC3 195 \u251C
		SendWithPrefix(L"\u251C", pMsg, pNewline, pEndline);
	}

	void Debug::NoteBig(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ╞ \xC6 198 \u255E
		SendWithPrefix(L"\u255E", pMsg, pNewline, pEndline);
	}

	void Debug::ProcessSmall(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ┼ \xC5 197 \u253C
		SendWithPrefix(L"\u253C", pMsg, pNewline, pEndline);
	}

	void Debug::ProcessBig(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ╪ \xD8 216 \u256A
		SendWithPrefix(L"\u256A", pMsg, pNewline, pEndline);
	}
#	pragma endregion

#	pragma region End
	void Debug::NoteSmallEnd(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// └ \xC0 192 \u2514
		SendWithPrefix(L"\u2514", pMsg, pNewline, pEndline);
	}

	void Debug::NoteBigEnd(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ╘ \xD4 212 \u2558
		SendWithPrefix(L"\u2558", pMsg, pNewline, pEndline);
	}

	void Debug::ProcessSmallEnd(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ┴ \xC1 193 \u2534
		SendWithPrefix(L"\u2534", pMsg, pNewline, pEndline);
	}

	void Debug::ProcessBigEnd(
		string const& pMsg, 
		bool pNewline, 
		bool pEndline) noexcept
	{
		// ╧ \xCF 207 \u2567
		SendWithPrefix(L"\u2567", pMsg, pNewline, pEndline);
	}
#	pragma endregion
}
