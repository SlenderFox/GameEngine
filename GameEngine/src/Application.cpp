#pragma region
#define WIN32_LEAN_AND_MEAN
#include "Application.hpp"
#include "Debug.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <Windows.h>	// Needed for Sleep()
#include <chrono>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"

using std::string;
using glm::vec3;
using glm::radians;
using glm::normalize;
#pragma endregion

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight) noexcept
{
	Engine::Application::GetApplication()->SetDimensions(pWidth, pHeight);
	Engine::Renderer::SetResolution(pWidth, pHeight);
}

namespace Engine
{
#	pragma region Variables
	const double Application::s_fixedDeltaTime = 1.0 / 60.0;

	Application* Application::s_application = nullptr;
	GLFWwindow* Application::s_windowRef = nullptr;
	bool Application::s_gladLoaded = false,
		Application::s_fullscreen = false;
	uint16_t Application::s_winWidth = 320U,
		Application::s_winHeight = 180U,
		Application::s_fps = 0U,
		Application::s_perSecondFrameCount = 0U;
	uint64_t Application::s_totalFrames = 0U;
	double Application::s_currentTime = 0.0,
		Application::s_prevTime = 0.0,
		Application::s_deltaTime = 0.0,
		Application::s_fixedTimer = 0.0,
		Application::s_frameTimer = 0.0;
	string Application::s_title = "Application";
	Application::ExitCode Application::s_exitCode = Application::ExitCode::Okay;
#	pragma endregion

	Application::Application()
	{
		// Prevents potential memory leak
		if (s_application)
		{
			Terminate();
			delete s_application;
		}

		// Allows debug to always be available
		Debug::Init();

		// Applies the static reference
		s_application = this;
	}

	Application::~Application()
	{
		Renderer::Destroy();
		delete Root::GetRoot();
	}

	const bool Application::GladLoaded() noexcept
	{
		return s_gladLoaded;
	}

	void Application::Quit() noexcept
	{
		glfwSetWindowShouldClose(Application::s_windowRef, true);
	}

	void Application::Terminate() noexcept
	{
		// End imgui
		if (ImGui::GetCurrentContext() != NULL)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		GetApplication()->Shutdown();
		glfwTerminate();
	}

	Application::ExitCode Application::Run()
	{
		if (Init())
		{
			// Preloads currentTime with an earlier time to prevent first frame weirdness
			s_currentTime = glfwGetTime() - s_fixedDeltaTime;

			// Render loop
			while (!(bool)glfwWindowShouldClose(s_windowRef))
			{
				UpdateFrameTimeData();

				// Input
				glfwPollEvents();
				ProcessInput();
				Input::Process();

				GetApplication()->Update();

				// Calls fixed update 60 times per second
				if (s_fixedTimer >= s_fixedDeltaTime)
				{
					s_fixedTimer -= s_fixedDeltaTime;
					GetApplication()->FixedUpdate();
				}

				// Skip drawing if minimised, restricts fps to 15
				if (glfwGetWindowAttrib(s_windowRef, GLFW_ICONIFIED) == GLFW_TRUE)
				{
					Sleep((DWORD)std::abs(50.0 - s_deltaTime));
					continue;
				}

				// Start a new imgui frame
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				GetApplication()->LateUpdate();

				Renderer::Draw();

				//ImGui::ShowDemoWindow();

				// Draw imgui last and on top
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Check and call events and swap the buffers
				glfwSwapBuffers(s_windowRef);
			}
		}

		Terminate();
		return s_exitCode;
	}

	bool Application::Init()
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		if (!SetupGLFW()) return false;	// Sets own exit code

		if (!SetupGlad())
		{
			s_exitCode = ExitCode::Fail_Glad;
			return false;
		}

		if (!Renderer::Init((float)s_winWidth / (float)s_winHeight))
		{
			s_exitCode = ExitCode::Fail_Renderer;
			return false;
		}

		if (!SetupImgui())
		{
			s_exitCode = ExitCode::Fail_Imgui;
			return false;
		}

		if (!Input::Init(s_windowRef))
		{
			s_exitCode = ExitCode::Fail_Input;
			return false;
		}
		// FIXME: Only works after input class is initialised
		//ImGui_ImplGlfw_InitForOpenGL(s_windowRef, true);

		if (!GetApplication()->Startup())
		{
			s_exitCode = ExitCode::Fail_Startup;
			return false;
		}

		// Calculates the time it took to start up
		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		Debug::Send("Started in " + std::to_string(elapsedTime.count()) + " seconds");

		return true;
	}

	bool Application::SetupGLFW()
	{
		// glfw: initialise and configure
		if (!glfwInit())
		{
			Debug::Send("Failed to initialise GLFW");
			s_exitCode = ExitCode::Fail_GLFW_Init;
			return false;
		}

		// ! Important glfw hints !
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation
		int monCount = 0;
		GLFWmonitor** monitors = glfwGetMonitors(&monCount);
		GLFWmonitor* primMon = monitors[0];
		const GLFWvidmode* vid = glfwGetVideoMode(primMon);

		glfwWindowHint(GLFW_RED_BITS, vid->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vid->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vid->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, vid->refreshRate);

		GLFWmonitor* mon = s_fullscreen ? primMon : nullptr;
		int wid = s_winWidth, hei = s_winHeight;

		s_windowRef = glfwCreateWindow(s_winWidth, s_winHeight, s_title.c_str(), mon, nullptr);

		if (!s_windowRef)
		{
			Debug::Send("Failed to create GLFW window");
			s_exitCode = ExitCode::Fail_GLFW_Window;
			return false;
		}

		glfwMakeContextCurrent(s_windowRef);
		glfwSetWindowSizeLimits(s_windowRef, 320, 180, GLFW_DONT_CARE, GLFW_DONT_CARE);
		//glfwSetWindowAspectRatio(s_windowRef, 16, 9);

		if (!s_fullscreen)
		{
			int monPosX, monPosY, monWidth, monHeight;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
#			ifdef _DEBUG
				// Moves the window to the left of the monitor
				glfwSetWindowPos(s_windowRef, 2, (int)((monHeight - s_winHeight) * 0.5f));
				// Moves the console to the right and resizes
				MoveWindow(GetConsoleWindow(), s_winWidth - 3, 0, 900, 1040, TRUE);
#			else
				// Moves the window to the center of the workarea
				glfwSetWindowPos(s_windowRef, (int)((monWidth - s_winWidth) * 0.5f),
					(int)((monHeight - s_winHeight) * 0.5f));
#			endif
		}

		glfwSetFramebufferSizeCallback(s_windowRef, framebuffer_size_callback);

		return true;
	}

	bool Application::SetupGlad()
	{
		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			Debug::Send("Failed to initialise GLAD");
			return false;
		}
		s_gladLoaded = true;
		return true;
	}

	bool Application::SetupImgui()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(s_windowRef, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::GetIO().DisplaySize.x = 1030.0f;
		ImGui::GetIO().DisplaySize.y = 650.0f;

		return true;
	}

	void Application::SetDimensions(uint16_t pWidth, uint16_t pHeight) noexcept
	{
		s_winWidth = pWidth;
		s_winHeight = pHeight;

		if (Renderer::s_camera && pWidth > 0 && pHeight > 0)
			UpdateCamera();

		Debug::Send(string("Dimensions set to " + std::to_string(s_winWidth) + ", " + std::to_string(s_winHeight)));
	}

	void Application::SetTitle(string pTitle) noexcept
	{
		s_title = pTitle;
		Debug::Send("Title set to \"" + s_title + "\"");
	}

	void Application::SetFullscreen(bool pFullscreen) noexcept
	{
		s_fullscreen = pFullscreen;
		Debug::Send("Fullscreen set to " + string(pFullscreen ? "true" : "false"));
	}

	void Application::UpdateCamera() noexcept
	{
		Renderer::s_camera->SetAspectRatio((float)s_winWidth / (float)s_winHeight);
		Renderer::s_camera->UpdateFovV();
	}

	void Application::UpdateFrameTimeData() noexcept
	{
		s_prevTime = s_currentTime;
		s_currentTime = glfwGetTime();
		s_deltaTime = s_currentTime - s_prevTime;
		s_fixedTimer += s_deltaTime;
		s_frameTimer += s_deltaTime;
		++s_totalFrames;
		++s_perSecondFrameCount;

		// Doing this allows me to updates fps as often as I want
		static const double secondsPerUpdate = 0.5;
		if (s_frameTimer >= secondsPerUpdate)
		{
			s_frameTimer -= secondsPerUpdate;
			s_fps = (uint16_t)((double)s_perSecondFrameCount / secondsPerUpdate);
			s_perSecondFrameCount = 0U;
			glfwSetWindowTitle(s_windowRef,
				(s_title + " | FPS: " + std::to_string(s_fps)).c_str());
		}
	}

	void Application::ProcessInput() noexcept
	{
		// TODO: Proper fullscreen toggle
		//if (Input::GetKey(Input::Key::Key_F11, Input::State::Press))
		//{
		//	s_fullscreen = true;
		//	GLFWmonitor* primMon = glfwGetPrimaryMonitor();
		//	int xp, yp;
		//	glfwGetMonitorPos(primMon, &xp, &yp);
		//	const GLFWvidmode* vid = glfwGetVideoMode(primMon);
		//	glfwSetWindowMonitor(s_windowRef, primMon, xp, yp, vid->width, vid->height, vid->refreshRate);
		//}
		//
		//if (Input::GetKey(Input::Key::Key_K, Input::State::Press))
		//{
		//	s_fullscreen = false;
		//	int monPosX, monPosY, monWidth, monHeight;
		//	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
		//	glfwSetWindowMonitor(s_windowRef, nullptr, 2, (int)((monHeight - s_winHeight) * 0.5f),
		//		s_winWidth, s_winHeight, GLFW_DONT_CARE);
		//	glfwSetWindowPos(s_windowRef, 2, (int)((monHeight - s_winHeight) * 0.5f));
		//}

		// End application
		if (Input::GetKeyState(Input::Key::Key_End, Input::State::Press)) Quit();
	}
}
