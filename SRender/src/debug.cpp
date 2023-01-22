#include "debug.hpp"
#include <io.h>
#include <fcntl.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#ifdef _DEBUG
#include "winclude.hpp"
# include <iostream>
 using std::wcout;
 using std::endl;
#endif

using std::string;

namespace srender
{
	namespace debug
	{
		/** List of prefixes:
		 * @note 00: Begin-Small-Note   : ┌ \xDA 218 \u250C
		 * @note 01: Begin-Small-Process: ┬ \xC2 194 \u252C
		 * @note 02: Begin-Large-Note   : ╒ \xD5 213 \u2552
		 * @note 03: Begin-Large-Process: ╤ \xD1 209 \u2564
		 * @note 04:   Mid-Small-Note   : ├ \xC3 195 \u251C
		 * @note 05:   Mid-Small-Process: ┼ \xC5 197 \u253C
		 * @note 06:   Mid-Large-Note   : ╞ \xC6 198 \u255E
		 * @note 07:   Mid-Large-Process: ╪ \xD8 216 \u256A
		 * @note 08:   End-Small-Note   : └ \xC0 192 \u2514
		 * @note 09:   End-Small-Process: ┴ \xC1 193 \u2534
		 * @note 10:   End-Large-Note   : ╘ \xD4 212 \u2558
		 * @note 11:   End-Large-Process: ╧ \xCF 207 \u2567
		 */
		const wchar_t *prefixes[12] = {
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

		void init(GLFWwindow *pWindow) noexcept
		{
			// Allows utf16 output to console
			_setmode(_fileno(stdout), _O_U16TEXT);

			ImGui::CreateContext();
			ImGui::StyleColorsDark();
			ImGui_ImplGlfw_InitForOpenGL(pWindow, false);
			ImGui_ImplOpenGL3_Init("#version 330");
			ImGui::GetIO().DisplaySize.x = 1030.0f;
			ImGui::GetIO().DisplaySize.y = 650.0f;
		}

		void terminate() noexcept
		{
			// End imgui
			if (ImGui::GetCurrentContext() != NULL)
			{
				ImGui_ImplOpenGL3_Shutdown();
				ImGui_ImplGlfw_Shutdown();
				ImGui::DestroyContext();
			}
		}

		void draw() noexcept
		{
			// Start a new imgui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//ImGui::ShowDemoWindow();

			// Finish and draw the frane
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		void send(
			const string pMsg,
			const bool pNewline,
			const bool pEndline
		) noexcept
		{
			#ifdef _DEBUG
				if (pNewline) wcout << '\n';
				wcout << pMsg.c_str();
				if (pEndline) wcout << endl;
			#endif
		}

		void send(
			const string pMsg,
			const wchar_t *pPrefix,
			const bool pNewline,
			const bool pEndline
		) noexcept
		{
			#ifdef _DEBUG
				if (pNewline) wcout << '\n';
				wcout << pPrefix << pMsg.c_str();
				if (pEndline) wcout << endl;
			#endif
		}

		void send(
			const string pMsg,
			const type pType,
			const impact pImpact,
			const stage pStage,
			const bool pNewline,
			const bool pEndline
		) noexcept
		{
			uint8_t pre = (uint8_t)pType + (uint8_t)pImpact + (uint8_t)pStage;
			send(pMsg, prefixes[pre], pNewline, pEndline);
		}

		void newLine() noexcept
		{
			#ifdef _DEBUG
				wcout << endl;
			#endif
		}

		#if defined(_DEBUG) && defined(__WINDOWS__)
			void MoveConsole(const uint16_t inWidth) noexcept
			{
				// Moves the console to the right and resizes
				MoveWindow(GetConsoleWindow(), inWidth - 3, 0, 900, 1040, TRUE);
			}
		#endif
	}
}
