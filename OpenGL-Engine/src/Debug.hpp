#pragma once
#ifdef _DEBUG
#include <string>
#include <iostream>

namespace Engine
{
	class Debug
	{
	private:
		static void PrintMsg(char pChar, std::string& pMsg, bool pNewline, bool pEndline) noexcept
		{
			if (pNewline) std::cout << '\n';
			printf(" %c%s", pChar, pMsg.c_str());
			if (pEndline) std::cout << std::endl;
		}

	public:
		static void InitSmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┌ \xDA 218
			PrintMsg('\xDA', pMsg, pNewline, pEndline);
		}

		static void InitBigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╒ \xD5 213
			PrintMsg('\xD5', pMsg, pNewline, pEndline);
		}

		static void InitSmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┬ \xC2 194
			PrintMsg('\xC2', pMsg, pNewline, pEndline);
		}

		static void InitBigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╤ \xD1 209
			PrintMsg('\xD1', pMsg, pNewline, pEndline);
		}

		static void SmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ├ \xC3 195
			PrintMsg('\xC3', pMsg, pNewline, pEndline);
		}

		static void BigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╞ \xC6 198
			PrintMsg('\xC6', pMsg, pNewline, pEndline);
		}

		static void SmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┼ \xC5 197
			PrintMsg('\xC5', pMsg, pNewline, pEndline);
		}

		static void BigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╪ \xD8 216
			PrintMsg('\xD8', pMsg, pNewline, pEndline);
		}

		static void EndSmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// └ \xC0 192
			PrintMsg('\xC0', pMsg, pNewline, pEndline);
		}

		static void EndBigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╘ \xD4 212
			PrintMsg('\xD4', pMsg, pNewline, pEndline);
		}

		static void EndSmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ┴ \xC1 193
			PrintMsg('\xC1', pMsg, pNewline, pEndline);
		}

		static void EndBigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			// ╧ \xCF 207
			PrintMsg('\xCF', pMsg, pNewline, pEndline);
		}
	};
}
#endif
