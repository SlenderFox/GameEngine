#pragma region
#include "Application.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <Windows.h>	// Needed for Sleep()
#include <chrono>

#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

using glm::vec3;
using std::string;
using glm::radians;
using glm::normalize;
#pragma endregion

// Called when the user resizes the window
void framebuffer_size_callback(GLFWwindow* pWindow, int pWidth, int pHeight)
{
	Engine::Application::GetApplication()->SetDimensions(pWidth, pHeight);
	glViewport(0, 0, pWidth, pHeight);
}

void mouse_callback(GLFWwindow* pWindow, double pPosX, double pPosY)
{
	Engine::Application::GetApplication()->MouseCallback(pPosX, pPosY);
}

void scroll_callback(GLFWwindow* window, double pOffsetX, double pOffsetY)
{
	Engine::Application::GetApplication()->ScrollCallback(pOffsetX, pOffsetY);
}

namespace Engine
{
	Application* Application::s_application = nullptr;

	Application::Application()
	{
		// Prevents potential memory leak
		if (s_application != nullptr)
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
		m_rendererInst->Destroy(m_gladLoaded);
		delete m_rendererInst;
		delete m_inputInst;
		// Don't need to delete m_window as it is handled by glfwTerminate()
	}

	void Application::Run(unsigned int pWidth, unsigned int pHeight, string pTitle, bool pFullscreen)
	{
		SetDimensions(pWidth, pHeight);

		if (Init(pTitle, pFullscreen))
		{
			m_currentTime = glfwGetTime() - 0.02f;

			const float radius = 10.0f;
			const float speed = 0.5f;

			// Render loop
			while (!glfwWindowShouldClose(m_window))
			{
				m_prevTime = m_currentTime;
				m_currentTime = glfwGetTime();
				m_deltaTime = m_currentTime - m_prevTime;
				m_fixedTimer += m_deltaTime;
				m_frameTimer += m_deltaTime;
				++m_frames;
				++m_framesPerSecond;

				// Doing this allows me to updates fps as often as I want
				float secondsPerUpdate = 0.5f;
				if (m_frameTimer >= secondsPerUpdate)
				{
					m_frameTimer -= secondsPerUpdate;
					m_fps = (unsigned int)((float)m_framesPerSecond / secondsPerUpdate);
					m_framesPerSecond = 0U;
					glfwSetWindowTitle(m_window, (m_title + " | FPS: " + std::to_string(m_fps)).c_str());
				}

				// Input
				glfwPollEvents();
				ProcessInput();

				// Calls fixed update 60 times per second
				if (m_fixedTimer >= m_fixedDeltaTime)
				{
					m_fixedTimer -= m_fixedDeltaTime;
					FixedUpdate(m_fixedDeltaTime);
				}

				//Update(m_deltaTime);

				// Skip drawing if minimised, restricts fps to 15
				if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) == GLFW_TRUE)
				{
					if (m_deltaTime < 50)
						Sleep(50);

					continue;
				}

				m_rendererInst->Draw(m_currentTime);

				// Check and call events and swap the buffers
				glfwSwapBuffers(m_window);
			}
		}

		Shutdown();
		glfwTerminate();
		return;
	}

	bool Application::Init(string pTitle, bool pFullscreen)
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		// glfw: initialise and configure
		if (glfwInit() == GLFW_FALSE)
		{
			#ifdef _DEBUG
			 cout << "GLFW failed to initialise" << endl;
			#endif
			return false;
		}

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
		if (m_window == nullptr)
		{
			#ifdef _DEBUG
			 cout << "Failed to create GLFW window" << endl;
			#endif
			return false;
		}
		glfwMakeContextCurrent(m_window);

		int monPosX, monPosY, monWidth, monHeight;
		glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
		#ifdef _DEBUG
		 // Moves the window to the lower right of the window
		 glfwSetWindowPos(m_window, 2, (int)((monHeight - m_winHeight) * 0.5f));
		 // Moves the console window
		 MoveWindow(GetConsoleWindow(), m_winWidth - 3, 0, 900, 600, TRUE);
		#else
		 // Moves the window to the center of the workarea
		 glfwSetWindowPos(m_window, (int)((monWidth - m_winWidth) * 0.5f), (int)((monHeight - m_winHeight) * 0.5f));
		#endif

		glfwSetWindowSizeLimits(m_window, 320, 180, GLFW_DONT_CARE, GLFW_DONT_CARE);
		//glfwSetWindowAspectRatio(m_window, 16, 9);

		glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwGetCursorPos(m_window, &m_mouseLastX, &m_mouseLastY);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

		glfwSetScrollCallback(m_window, scroll_callback);

		//glfwSetWindowAspectRatio(m_window, 16, 9);
		//int* monCount = 0; 
		//GLFWmonitor** monitors = glfwGetMonitors(monCount);

		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			#ifdef _DEBUG
			 cout << "Failed to initialise GLAD" << endl;
			#endif
			return false;
		}
		m_gladLoaded = true;

		// Initialises the renderer
		m_rendererInst->Init((float)m_winWidth / (float)m_winHeight);

		if (!Startup())
			return false;

		// Calculates the time I took to start up
		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		#ifdef _DEBUG
		 cout << "Started in " << elapsedTime.count() << " seconds\n";
		#endif

		return true;
	}

	void Application::SetDimensions(unsigned int pWidth, unsigned int pHeight)
	{
		m_winWidth = pWidth;
		m_winHeight = pHeight;

		if (m_rendererInst->m_camera != nullptr && pWidth > 0 && pHeight > 0)
			UpdateCamera();
	}

	void Application::UpdateCamera()
	{
		m_rendererInst->m_camera->SetAspectRatio((float)m_winWidth / (float)m_winHeight);
		m_rendererInst->m_camera->UpdateFovV();
	}

	void Application::MouseCallback(double pPosX, double pPosY)
	{
		double offsetX = pPosX - m_mouseLastX;
		double offsetY = pPosY - m_mouseLastY;
		m_mouseLastX = pPosX;
		m_mouseLastY = pPosY;
		const double sens = 0.05f;
		offsetX *= sens;
		offsetY *= sens;
		m_yaw += offsetX;
		m_pitch += offsetY;
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		else if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		// The forward direction of the camera
		vec3 forward = vec3();
		forward.x = (float)cos(radians(m_yaw)) * (float)cos(radians(m_pitch));
		forward.y = (float)sin(radians(m_pitch));
		forward.z = (float)sin(radians(m_yaw)) * (float)cos(radians(m_pitch));
		forward = normalize(forward);
		m_rendererInst->m_camera->SetForward(forward);
	}

	void Application::ScrollCallback(double pOffsetX, double pOffsetY)
	{
		m_rendererInst->m_camera->ModifyFovH((float)pOffsetY * -3.0f);
	}

	void Application::ProcessInput()
	{
		// End application
		if (glfwGetKey(m_window, GLFW_KEY_END) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_window, true);
		// Render triangles normally
		if (glfwGetKey(m_window, GLFW_KEY_F1) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// Render triangles as lines
		if (glfwGetKey(m_window, GLFW_KEY_F2) == GLFW_PRESS)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Toggle fullscreen
		// if (glfwGetKey(m_window, GLFW_KEY_F11) == GLFW_PRESS)
		// { }

		// Spotlight cone
		if (glfwGetKey(m_window, GLFW_KEY_T) == GLFW_PRESS)
		{
			m_rendererInst->ModifyAllSpotlightAngles(0.05f);
		}
		if (glfwGetKey(m_window, GLFW_KEY_G) == GLFW_PRESS)
		{
			m_rendererInst->ModifyAllSpotlightAngles(-0.05f);
		}
		// Spotlight blur
		if (glfwGetKey(m_window, GLFW_KEY_Y) == GLFW_PRESS)
		{
			m_rendererInst->ModifyAllSpotlightBlurs(-0.005f);
		}
		if (glfwGetKey(m_window, GLFW_KEY_H) == GLFW_PRESS)
		{
			m_rendererInst->ModifyAllSpotlightBlurs(0.005f);
		}

		float moveSpeed = 8;
		// SlowDown
		if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			moveSpeed *= 0.2f;
		// SpeedUp
		else if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			moveSpeed *= 3;

		vec3 translation = vec3();
		// Forwards
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
			translation += moveSpeed * (float)m_deltaTime * (vec3)m_rendererInst->m_camera->GetForward();
		// Backwards
		if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
			translation -= moveSpeed * (float)m_deltaTime * (vec3)m_rendererInst->m_camera->GetForward();
		// Left
		if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
			translation += moveSpeed * (float)m_deltaTime * (vec3)m_rendererInst->m_camera->GetRight();
		// Right
		if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
			translation -= moveSpeed * (float)m_deltaTime * (vec3)m_rendererInst->m_camera->GetRight();
		// Up
		if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			translation += moveSpeed * (float)m_deltaTime * (vec3)m_rendererInst->m_camera->GetUp();
		// Down
		if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS)
			translation -= moveSpeed * (float)m_deltaTime * (vec3)m_rendererInst->m_camera->GetUp();

		m_rendererInst->m_camera->Translate(translation);
	}
}
