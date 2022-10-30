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
	const wchar_t* Debug::s_prefixes[12] =
	{
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

	void Debug::Init() noexcept
	{
		// Allows utf16 output
		_setmode(_fileno(stdout), _O_U16TEXT);
	}

	void Debug::Send(
		string pMsg,
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
		string pMsg,
		const wchar_t* pPrefix,
		bool pNewline,
		bool pEndline) noexcept
	{
#		ifdef _DEBUG
			if (pNewline) wcout << '\n';
			wcout << pPrefix << pMsg.c_str();
			if (pEndline) wcout << endl;
#		endif
	}

	void Debug::SendWithPrefix(
		string pMsg,
		Type pType,
		Impact pImpact,
		Stage pStage,
		bool pNewline,
		bool pEndline) noexcept
	{
		uint8_t pre = (uint8_t)pType + (uint8_t)pImpact + (uint8_t)pStage;
		SendWithPrefix(pMsg, s_prefixes[pre], pNewline, pEndline);
	}

	void Debug::NewLine() noexcept
	{
#		ifdef _DEBUG
			wcout << endl;
#		endif
	}
}
