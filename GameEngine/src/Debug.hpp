#pragma once
#include <string>

namespace Engine
{
	struct Debug
	{

	public:
		// Can be removed when transitioned fully to imgui
		static void Init() noexcept;

		static void Send(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void SendWithPrefix(
			const wchar_t* pPrefix,
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;

		static void NewLine() noexcept;

		// TODO: Look into converging these into a single function with enum selection

#		pragma region Start
		static void NoteSmallStart(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void NoteBigStart(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void ProcessSmallStart(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void ProcessBigStart(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
#		pragma endregion

#		pragma region Mid
		static void NoteSmall(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void NoteBig(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void ProcessSmall(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void ProcessBig(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
#		pragma endregion

#		pragma region End
		static void NoteSmallEnd(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void NoteBigEnd(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void ProcessSmallEnd(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void ProcessBigEnd(
			std::string const& pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
#		pragma endregion
	};
}
