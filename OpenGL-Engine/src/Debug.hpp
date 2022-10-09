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
			std::cout << ' ' << pChar << pMsg;
			if (pEndline) std::cout << std::endl;
		}

	public:
		static void InitSmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xDA', pMsg, pNewline, pEndline);
		}

		static void InitBigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xD5', pMsg, pNewline, pEndline);
		}

		static void InitSmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xC2', pMsg, pNewline, pEndline);
		}

		static void InitBigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xD1', pMsg, pNewline, pEndline);
		}

		static void SmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xC3', pMsg, pNewline, pEndline);
		}

		static void BigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xC6', pMsg, pNewline, pEndline);
		}

		static void SmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xC5', pMsg, pNewline, pEndline);
		}

		static void BigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xD8', pMsg, pNewline, pEndline);
		}

		static void EndSmallNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xC0', pMsg, pNewline, pEndline);
		}

		static void EndBigNote(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xD4', pMsg, pNewline, pEndline);
		}

		static void EndSmallProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xC1', pMsg, pNewline, pEndline);
		}

		static void EndBigProcess(std::string pMsg, bool pNewline = true, bool pEndline = true) noexcept
		{
			PrintMsg('\xD0', pMsg, pNewline, pEndline);
		}
	};
}
#endif
