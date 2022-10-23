#pragma once
#include <string>

namespace Engine
{
	class Debug
	{
	private:
		static void SendWithPrefix(const wchar_t* pPrefix, std::string& pMsg, bool pNewline, bool pEndline) noexcept;

	public:
		static void Init() noexcept;

		static void Send(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void NewLine() noexcept;

#		pragma region Start
		static void StartSmallNote(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void StartBigNote(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void StartSmallProcess(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void StartBigProcess(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
#		pragma endregion
#		pragma region Mid
		static void SmallNote(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void BigNote(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void SmallProcess(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void BigProcess(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
#		pragma endregion
#		pragma region End
		static void EndSmallNote(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void EndBigNote(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void EndSmallProcess(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
		static void EndBigProcess(std::string pMsg, bool pNewline = false, bool pEndline = true) noexcept;
#		pragma endregion
	};
}
