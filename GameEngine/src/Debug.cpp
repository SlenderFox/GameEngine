#pragma region
#include "Debug.hpp"
#include <io.h>
#include <fcntl.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

#ifdef _DEBUG
# include <iostream>
 using std::wcout;
 using std::endl;
#endif

using std::string;
#pragma endregion

namespace Engine
{
	const wchar_t* Debug::s_prefixes[12] = {
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

	void Debug::Init(GLFWwindow* pWindow) noexcept
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

	void Debug::Terminate() noexcept
	{
		// End imgui
		if (ImGui::GetCurrentContext() != NULL)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}
	}

	void Debug::Draw() noexcept
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

	void Debug::Send(
		const string pMsg,
		const bool pNewline,
		const bool pEndline) noexcept
	{
#		ifdef _DEBUG
			if (pNewline) wcout << '\n';
			wcout << pMsg.c_str();
			if (pEndline) wcout << endl;
#		endif
	}

	void Debug::Send(
		const string pMsg,
		const wchar_t* pPrefix,
		const bool pNewline,
		const bool pEndline) noexcept
	{
#		ifdef _DEBUG
			if (pNewline) wcout << '\n';
			wcout << pPrefix << pMsg.c_str();
			if (pEndline) wcout << endl;
#		endif
	}

	void Debug::Send(
		const string pMsg,
		const Type pType,
		const Impact pImpact,
		const Stage pStage,
		const bool pNewline,
		const bool pEndline) noexcept
	{
		uint8_t pre = (uint8_t)pType + (uint8_t)pImpact + (uint8_t)pStage;
		Send(pMsg, s_prefixes[pre], pNewline, pEndline);
	}

	void Debug::NewLine() noexcept
	{
#		ifdef _DEBUG
			wcout << endl;
#		endif
	}
}
