#pragma region
#include "Application.hpp"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Windows.h>	// Needed for Sleep()
#ifdef _DEBUG
 #include <iostream>
 using std::cout;
 using std::endl;
#endif

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
#pragma endregion

namespace Engine
{
	Application* Application::sm_appRef = nullptr;

	Application::Application()
	{
		// Prevents potential memory leak
		if (sm_appRef != nullptr)
			delete sm_appRef;

		// Applies the static reference
		sm_appRef = this;

		// Need to be instantiated in here to avoid errors if glfw or glad fail
		m_rendererInst = Renderer::GetInstance();
		m_inputInst = Input::GetInstance();
	}

	Application::~Application()
	{
		m_rendererInst->Destroy(m_gladLoaded);
		delete m_rendererInst;
		delete m_inputInst;
		delete m_cameraRef;
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

				{
					// Doing this allows me to updates fps as often as I want
					float secondsPerUpdate = 0.5f;
					if (m_frameTimer >= secondsPerUpdate)
					{
						m_frameTimer -= secondsPerUpdate;
						m_fps = (unsigned int)((float)m_framesPerSecond / secondsPerUpdate);
						m_framesPerSecond = 0U;
// #ifdef _DEBUG
//                         printf("fps: %*u, current frame time: %3.7f \n", 2, m_fps, m_deltaTime);
// #endif
					}
				}

				// Input
				glfwPollEvents();
				m_inputInst->ProcessInput(m_window);

				// Temporary
				ProcessInput();

				Update(m_deltaTime);

				// Calls fixed update 60 times per second
				if (m_fixedTimer >= m_fixedDeltaTime)
				{
					m_fixedTimer -= m_fixedDeltaTime;
					FixedUpdate(m_fixedDeltaTime);
				}

				// Skip drawing if minimised, restricts fps to 15
				if (glfwGetWindowAttrib(m_window, GLFW_ICONIFIED) != 0)
				{
					if (m_deltaTime < 50)
						Sleep(50);

					continue;
				}

				//float camX = (float)sin(m_currentTime * speed) * radius;
				//float camZ = (float)cos(m_currentTime * speed) * radius;

				//m_cameraRef->SetView(glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));

				m_rendererInst->Draw(m_cameraRef->GetWorldToCameraMatrix(), m_currentTime);

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
#ifdef _DEBUG
		// Moves the console window
		MoveWindow(GetConsoleWindow(), 0, 0, 600, 600, TRUE);
#endif

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
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Already set by default
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
		//glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
		//glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
		//glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);

		// glfw window creation
		m_window = glfwCreateWindow(m_winWidth, m_winHeight, pTitle.c_str(),
			(pFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
		if (m_window == nullptr)
		{
#ifdef _DEBUG
			cout << "Failed to create GLFW window" << endl;
#endif
			return false;
		}
		glfwMakeContextCurrent(m_window);

		{
			// Moves the window to the center of the workarea
			int monPosX, monPosY, monWidth, monHeight;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
			glfwSetWindowPos(m_window, (int)((monWidth - m_winWidth) * 0.5f), (int)((monHeight - m_winHeight) * 0.5f));
		}

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
		m_rendererInst->Init();

		m_cameraRef = new Camera((float)m_winWidth / (float)m_winHeight, 75.0f);
		//UpdateCamera();
		m_cameraRef->SetClearColour(0.2f, 0.2f, 0.2f);
		m_cameraRef->SetPosition({ 0.0f, 0.0f, 6.0f });

		if (!Startup())
			return false;

		return true;
	}

	void Application::SetDimensions(unsigned int pWidth, unsigned int pHeight)
	{
		m_winWidth = pWidth;
		m_winHeight = pHeight;

		if (m_cameraRef != nullptr && pWidth > 0 && pHeight > 0)
		{
			UpdateCamera();
		}
	}

	void Application::UpdateCamera()
	{
		m_cameraRef->SetAspectRatio((float)m_winWidth / (float)m_winHeight);
		m_cameraRef->UpdateFovV();
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
		forward.x = (float)cos(glm::radians(m_yaw)) * (float)cos(glm::radians(m_pitch));
		forward.y = (float)sin(glm::radians(m_pitch));
		forward.z = (float)sin(glm::radians(m_yaw)) * (float)cos(glm::radians(m_pitch));
		forward = glm::normalize(forward);
		vec3 right = glm::normalize(glm::cross(vec3(0, 1, 0), forward));
		vec3 up = glm::cross(forward, right);

		m_cameraRef->SetRight(right);
		m_cameraRef->SetUp(up);
		m_cameraRef->SetForward(forward);
	}

	void Application::ScrollCallback(double pOffsetX, double pOffsetY)
	{
		m_cameraRef->ModifyFovH((float)pOffsetY * -3.0f);
	}

	void Application::ProcessInput()
	{
		float speed;
		vec3 translation = vec3();

		// Toggle fullscreen
		if (glfwGetKey(m_window, GLFW_KEY_F11) == GLFW_PRESS)
		{

		}

		// SlowDown
		if (glfwGetKey(m_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			speed = 2;
		// SpeedUp
		else if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			speed = 16;
		else
			speed = 8;

		// Forwards
		if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
			translation += speed * (float)m_deltaTime * m_cameraRef->GetForward();
		// Backwards
		if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
			translation -= speed * (float)m_deltaTime * m_cameraRef->GetForward();
		// Left
		if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
			translation += speed * (float)m_deltaTime * m_cameraRef->GetRight();
		// Right
		if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
			translation -= speed * (float)m_deltaTime * m_cameraRef->GetRight();
		// Up
		if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
			translation += speed * (float)m_deltaTime * m_cameraRef->GetUp();
		// Down
		if (glfwGetKey(m_window, GLFW_KEY_C) == GLFW_PRESS)
			translation -= speed * (float)m_deltaTime * m_cameraRef->GetUp();

		m_cameraRef->Translate(translation);
	}
}
