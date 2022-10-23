#pragma region
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

using glm::vec3;
using std::string;
using glm::radians;
using glm::normalize;
#pragma endregion

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight) noexcept
{
	Engine::Application::GetApplication()->SetDimensions(pWidth, pHeight);
	glViewport(0, 0, pWidth, pHeight);
}

namespace Engine
{
	// Static 

	Application* Application::s_application = nullptr;
	bool Application::s_gladLoaded = false;

	const bool Application::GladLoaded() noexcept
	{
		return s_gladLoaded;
	}

	void Application::Quit() noexcept
	{
		glfwSetWindowShouldClose(Application::GetApplication()->m_window, true);
	}

	void Application::MouseCallback(double& pPosX, double& pPosY) noexcept
	{
		Application* app = Application::GetApplication();
		double offsetX = pPosX - app->m_mouseLastX;
		double offsetY = pPosY - app->m_mouseLastY;
		app->m_mouseLastX = pPosX;
		app->m_mouseLastY = pPosY;
		const double sens = 0.05f;
		offsetX *= sens;
		offsetY *= sens;
		app->m_yaw += offsetX;
		app->m_pitch += offsetY;
		if (app->m_pitch > 89.0f)
			app->m_pitch = 89.0f;
		else if (app->m_pitch < -89.0f)
			app->m_pitch = -89.0f;

		// The forward direction of the camera
		vec3 forward = vec3();
		forward.x = (float)cos(radians(app->m_yaw)) * (float)cos(radians(app->m_pitch));
		forward.y = (float)sin(radians(app->m_pitch));
		forward.z = (float)sin(radians(app->m_yaw)) * (float)cos(radians(app->m_pitch));
		forward = normalize(forward);
		Renderer::GetInstance()->m_camera->SetForward(forward);
	}

	void Application::ScrollCallback(double& pOffsetX, double& pOffsetY) noexcept
	{
		Renderer::GetInstance()->m_camera->ModifyFovH((float)pOffsetY * -3.0f);
	}

	// Member

	Application::Application()
	{
		// Prevents potential memory leak
		if (s_application)
		{
			s_application->Shutdown();
			glfwTerminate();
			delete s_application;
		}

		// Applies the static reference
		s_application = this;

		// Need to be instantiated in here to avoid errors if glfw or glad fail
		m_rendererInst = Renderer::GetInstance();
		m_inputInst = Input::GetInstance();
	}

	Application::~Application()
	{
		delete m_rendererInst;
		delete m_inputInst;
		delete Root::GetRoot();
		// Don't need to delete m_window as it is handled by glfwTerminate()
	}

	Application::ExitCode Application::Run(const uint16_t& pWidth, 
	 const uint16_t& pHeight, const string& pTitle, const bool& pFullscreen)
	{
		SetDimensions(pWidth, pHeight);

		if (Init(pTitle, pFullscreen))
		{
			// Preloads currentTime with an earlier time to prevent first frame weirdness
			m_currentTime = glfwGetTime() - m_fixedDeltaTime;

			// Render loop
			while (!(bool)glfwWindowShouldClose(m_window))
			{
				UpdateFrameTimeData();

				// Input
				glfwPollEvents();
				ProcessInput();
				m_inputInst->Process();

				Update();

				// Calls fixed update 60 times per second
				if (m_fixedTimer >= m_fixedDeltaTime)
				{
					m_fixedTimer -= m_fixedDeltaTime;
					FixedUpdate();
				}

				// Skip drawing if minimised, restricts fps to 15
				if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) == GLFW_TRUE)
				{
					Sleep((DWORD)std::abs(50.0 - m_deltaTime));
					continue;
				}

				// Start a new imgui frame
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				LateUpdate();

				m_rendererInst->Draw();
				
				ImGui::ShowDemoWindow();
				
				// Draw imgui last and on top
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				// Check and call events and swap the buffers
				glfwSwapBuffers(m_window);
			}
		}

		// End imgui
		if (ImGui::GetCurrentContext() != NULL)
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
			ImGui::DestroyContext();
		}

		Shutdown();
		glfwTerminate();
		return m_exitCode;
	}

	bool Application::Init(const string& pTitle, bool pFullscreen)
	{
		// Must be first to init
		Debug::Init();
		auto startTime = std::chrono::high_resolution_clock::now();

		if (!SetupGLFW(pTitle, pFullscreen)) return false;	// Sets own exit code

		if (!SetupGlad())
		{
			m_exitCode = ExitCode::Fail_Glad;
			return false;
		}

		// Initialises the renderer
		if (!m_rendererInst->Init((float)m_winWidth / (float)m_winHeight))
		{
			m_exitCode = ExitCode::Fail_Renderer;
			return false;
		}

		if (!SetupImgui())
		{
			m_exitCode = ExitCode::Fail_Imgui;
			return false;
		}

		if (!m_inputInst->Init(m_window))
		{
			m_exitCode = ExitCode::Fail_Input;
			return false;
		}
		// FIXME: For some reason only works after input class is initialised
		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		
		//m_inputInst->AddMouseCallback(MouseCallback);
		//m_inputInst->AddSrollCallback(ScrollCallback);

		if (!Startup())
		{
			m_exitCode = ExitCode::Fail_Startup;
			return false;
		}

		// Calculates the time it took to start up
		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		Debug::Send("Started in " + std::to_string(elapsedTime.count()) + " seconds");

		return true;
	}

	bool Application::SetupGLFW(const std::string& pTitle, bool pFullscreen)
	{
		// glfw: initialise and configure
		if (!glfwInit())
		{
			Debug::Send("Failed to initialise GLFW");
			m_exitCode = ExitCode::Fail_GLFW_Init;
			return false;
		}

		// ! Important glfw hints !
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Already set by default
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		//glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
		//glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

		// glfw window creation
		m_window = glfwCreateWindow(m_winWidth, m_winHeight, (m_title = pTitle).c_str(),
			(pFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
		if (!m_window)
		{
			Debug::Send("Failed to create GLFW window");
			m_exitCode = ExitCode::Fail_GLFW_Window;
			return false;
		}
		glfwMakeContextCurrent(m_window);

		glfwSetWindowSizeLimits(m_window, 320, 180, GLFW_DONT_CARE, GLFW_DONT_CARE);
		//glfwSetWindowAspectRatio(m_window, 16, 9);
		//int monCount = 0; 
		//GLFWmonitor** monitors = glfwGetMonitors(&monCount);

		{
			int monPosX, monPosY, monWidth, monHeight;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
#			ifdef _DEBUG
			 // Moves the window to the lower right of the window
			 glfwSetWindowPos(m_window, 2, (int)((monHeight - m_winHeight) * 0.5f));
			 // Moves the console and resizes
			 MoveWindow(GetConsoleWindow(), m_winWidth - 3, 0, 900, 1040, TRUE);
#			else
			 // Moves the window to the center of the workarea
			 glfwSetWindowPos(m_window, (int)((monWidth - m_winWidth) * 0.5f), (int)((monHeight - m_winHeight) * 0.5f));
#			endif
		}

		//Callbacks
		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

		// TODO: Remove
		glfwGetCursorPos(m_window, &m_mouseLastX, &m_mouseLastY);
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
		//ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::GetIO().DisplaySize.x = 1030.0f;
		ImGui::GetIO().DisplaySize.y = 650.0f;

		return true;
	}

	void Application::SetDimensions(const uint16_t& pWidth, const uint16_t& pHeight) noexcept
	{
		m_winWidth = pWidth;
		m_winHeight = pHeight;

		if (m_rendererInst->m_camera && pWidth > 0 && pHeight > 0)
			UpdateCamera();
	}

	void Application::UpdateCamera() noexcept
	{
		m_rendererInst->m_camera->SetAspectRatio((float)m_winWidth / (float)m_winHeight);
		m_rendererInst->m_camera->UpdateFovV();
	}

	void Application::UpdateFrameTimeData() noexcept
	{
		m_prevTime = m_currentTime;
		m_currentTime = glfwGetTime();
		m_deltaTime = m_currentTime - m_prevTime;
		m_fixedTimer += m_deltaTime;
		m_frameTimer += m_deltaTime;
		++m_totalFrames;
		++m_perSecondFrameCount;

		// Doing this allows me to updates fps as often as I want
		static const double secondsPerUpdate = 0.5;
		if (m_frameTimer >= secondsPerUpdate)
		{
			m_frameTimer -= secondsPerUpdate;
			m_fps = (uint16_t)((double)m_perSecondFrameCount / secondsPerUpdate);
			m_perSecondFrameCount = 0U;
			glfwSetWindowTitle(m_window,
			 (m_title + " | FPS: " + std::to_string(m_fps)).c_str());
		}
	}

	void Application::ProcessInput() noexcept
	{
		// TODO: Add fullscreen toggle
		// End application
		if (m_inputInst->GetKey(Input::Key::Key_End, Input::State::Press)) Quit();
	}
}
