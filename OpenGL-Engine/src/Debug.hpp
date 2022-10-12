#pragma once
#ifdef _DEBUG
#include <string>
#include <iostream>

namespace Engine
{
	class Debug
	{
	private:
		static void PrintMsg(const wchar_t* pChar, std::string& pMsg, bool pNewline, bool pEndline) noexcept
		{
			if (pNewline) std::wcout << '\n';
			//printf(" %c%s", pChar, pMsg.c_str());
			std::wcout << L" " << pChar << pMsg.c_str();
			if (pEndline) std::wcout << std::endl;
		}

	public:
		static void InitSmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┌ \xDA 218 \u250C
			PrintMsg(L"\u250C", pMsg, pNewline, pEndline);
		}

		static void InitBigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╒ \xD5 213 \u2552
			PrintMsg(L"\u2552", pMsg, pNewline, pEndline);
		}

		static void InitSmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┬ \xC2 194 \u252C
			PrintMsg(L"\u252C", pMsg, pNewline, pEndline);
		}

		static void InitBigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╤ \xD1 209 \u2564
			PrintMsg(L"\u2564", pMsg, pNewline, pEndline);
		}

		static void SmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ├ \xC3 195 \u251C
			PrintMsg(L"\u251C", pMsg, pNewline, pEndline);
		}

		static void BigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╞ \xC6 198 \u255E
			PrintMsg(L"\u255E", pMsg, pNewline, pEndline);
		}

		static void SmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┼ \xC5 197 \u253C
			PrintMsg(L"\u253C", pMsg, pNewline, pEndline);
		}

		static void BigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╪ \xD8 216 \u256A
			PrintMsg(L"\u256A", pMsg, pNewline, pEndline);
		}

		static void EndSmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// └ \xC0 192 \u2514
			PrintMsg(L"\u2514", pMsg, pNewline, pEndline);
		}

		static void EndBigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╘ \xD4 212 \u2558
			PrintMsg(L"\u2558", pMsg, pNewline, pEndline);
		}

		static void EndSmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┴ \xC1 193 \u2534
			PrintMsg(L"\u2534", pMsg, pNewline, pEndline);
		}

		static void EndBigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╧ \xCF 207 \u2567
			PrintMsg(L"\u2567", pMsg, pNewline, pEndline);
		}
	};
}
#endif
