/**
 * @file Debug.hpp
 * @author SlenderFox
 * @brief Handles debug output
 *
 * -----Prefixes:
 * 00: Begin-Small-Note   : ┌ \xDA 218 \u250C
 * 01: Begin-Small-Process: ┬ \xC2 194 \u252C
 * 02: Begin-Large-Note   : ╒ \xD5 213 \u2552
 * 03: Begin-Large-Process: ╤ \xD1 209 \u2564
 * 04: Mid  -Small-Note   : ├ \xC3 195 \u251C
 * 05: Mid  -Small-Process: ┼ \xC5 197 \u253C
 * 06: Mid  -Large-Note   : ╞ \xC6 198 \u255E
 * 07: Mid  -Large-Process: ╪ \xD8 216 \u256A
 * 08: End  -Small-Note   : └ \xC0 192 \u2514
 * 09: End  -Small-Process: ┴ \xC1 193 \u2534
 * 10: End  -Large-Note   : ╘ \xD4 212 \u2558
 * 11: End  -Large-Process: ╧ \xCF 207 \u2567
 */
#pragma once
#include <string>

namespace Engine
{
	struct Debug
	{
		enum class Type: uint8_t
		{
			Note = 0,
			Process = 1
		};

		enum class Impact: uint8_t
		{
			Small = 0,
			Large = 2
		};

		enum class Stage: uint8_t
		{
			Begin = 0,
			Mid = 4,
			End = 8
		};

		static const wchar_t* s_prefixes[12];

		// Can be removed when transitioned fully to imgui
		static void Init() noexcept;

		static void Send(
			std::string pMsg,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void Send(
			std::string pMsg,
			const wchar_t* pPrefix,
			bool pNewline = false,
			bool pEndline = true) noexcept;
		static void Send(
			std::string pMsg,
			Type pType,
			Impact pImpact,
			Stage pStage,
			bool pNewline = false,
			bool pEndline = true) noexcept;

		static void NewLine() noexcept;
	};
}
