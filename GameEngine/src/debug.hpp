#pragma once
#include <string>

struct GLFWwindow;

namespace engine
{
	namespace debug
	{
		enum class type: uint8_t
		{
			Note = 0,
			Process = 1
		};

		enum class impact: uint8_t
		{
			Small = 0,
			Large = 2
		};

		enum class stage: uint8_t
		{
			Begin = 0,
			Mid = 4,
			End = 8
		};

		void init(GLFWwindow *pWindow) noexcept;
		void terminate() noexcept;
		void draw() noexcept;

		void send(
			const std::string pMsg,
			const bool pNewline = false,
			const bool pEndline = true
		) noexcept;
		void send(
			const std::string pMsg,
			const wchar_t *pPrefix,
			const bool pNewline = false,
			const bool pEndline = true
		) noexcept;
		void send(
			const std::string pMsg,
			const type pType,
			const impact pImpact,
			const stage pStage,
			const bool pNewline = false,
			const bool pEndline = true
		) noexcept;

		void newLine() noexcept;

		#ifdef _DEBUG
			void MoveConsole(const uint16_t inWidth) noexcept;
		#endif
	};
}
