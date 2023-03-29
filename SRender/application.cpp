#include "application.hpp"
#include "debug.hpp"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include <chrono>
//#include <thread>

using std::string;
using std::to_string;
using glm::vec3;
using glm::radians;
using glm::normalize;

namespace srender
{
	application *application::s_application = nullptr;
	GLFWwindow *application::s_windowRef = nullptr;
	bool application::s_gladLoaded = false,
		application::s_fullscreen = false;
	uint16_t application::s_w_width = 320U,
		application::s_w_height = 180U,
		application::s_fps = 0U,
		application::s_perSecondFrameCount = 0U;
	uint64_t application::s_totalFrames = 0U;
	double application::s_currentTime = 0.0,
		application::s_prevTime = 0.0,
		application::s_deltaTime = 0.0,
		application::s_fixedTimer = 0.0,
		application::s_frameTimer = 0.0;
	string application::s_title = "SRender";
	string application::s_appLocation = "";
	application::exitCode application::s_exitCode = application::exitCode::okay;

	application *application::getApplication() noexcept
	{ return s_application; }

	const bool application::gladLoaded() noexcept
	{
		return s_gladLoaded;
	}

	void application::quit() noexcept
	{
		glfwSetWindowShouldClose(application::s_windowRef, true);
	}

	application::application() noexcept
	{
		// Prevents potential memory leak
		if (s_application)
		{
			s_application->terminate();
			delete s_application;
		}

		// Applies the static reference
		s_application = this;
	}

	void application::terminate() noexcept
	{
		getApplication()->shutdown();
		renderer::terminate();
		glfwTerminate();
		delete root::getRoot();
	}

	application::exitCode application::run()
	{
		if (init())
		{
			// Preloads currentTime with an earlier time to prevent first frame weirdness
			s_currentTime = glfwGetTime() - s_fixedDeltaTime;

			// Render loop
			while (!(bool)glfwWindowShouldClose(s_windowRef))
			{
				updateFrameTimeData();

				// Input
				glfwPollEvents();
				processInput();
				input::process();

				getApplication()->update();

				// Calls fixed update 60 times per second
				if (s_fixedTimer >= s_fixedDeltaTime)
				{
					s_fixedTimer -= s_fixedDeltaTime;
					getApplication()->fixedUpdate();
				}

				// Skip drawing if minimised, restricts fps to 15
				if ((bool)glfwGetWindowAttrib(s_windowRef, GLFW_ICONIFIED))
				{
					// TODO: Figure out why sleeping the thread freezes when unminimised
					//std::this_thread::sleep_for(std::chrono::milliseconds(66));

					// Waste time
					double waster = 0.2893652150526226374;
					while (glfwGetTime() < s_currentTime + 0.06666666666666666)
					{
						waster /= waster;
					}
					continue;
				}

				getApplication()->fateUpdate();

				renderer::draw();

				// Check and call events and swap the buffers
				glfwSwapBuffers(s_windowRef);
			}
		}

		terminate();
		return s_exitCode;
	}

	bool application::init()
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		if (!setupGLFW()) return false;	// Sets own exit code

		if (!setupGlad())
		{
			s_exitCode = exitCode::fail_Glad;
			return false;
		}

		// Has to be initialised after glfw and glad
		debug::init();

		if (!renderer::init((float)s_w_width / (float)s_w_height))
		{
			s_exitCode = exitCode::fail_Renderer;
			return false;
		}

		if (!input::init(s_windowRef))
		{
			s_exitCode = exitCode::fail_Input;
			return false;
		}

		if (!getApplication()->startup())
		{
			s_exitCode = exitCode::fail_Startup;
			return false;
		}

		// Calculates the time it took to start up
		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		debug::send("Started in " + to_string(elapsedTime.count()) + " seconds");

		return true;
	}

	bool application::setupGLFW()
	{
		// glfw: initialise and configure
		if (!glfwInit())
		{
			s_exitCode = exitCode::fail_GLFW_Init;
			const char *description;
			int code = glfwGetError(&description);
			assert(description != nullptr);
			debug::send("Code: " + std::to_string(code) + "\nDescription: " + description + "\n");
			return false;
		}

		// ! Important glfw hints !
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// glfw window creation
		int monCount = 0;
		GLFWmonitor **monitors = glfwGetMonitors(&monCount);
		GLFWmonitor *primMon = monitors[0];
		const GLFWvidmode *vid = glfwGetVideoMode(primMon);

		glfwWindowHint(GLFW_RED_BITS, vid->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vid->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vid->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, vid->refreshRate);

		GLFWmonitor *mon = s_fullscreen ? primMon : nullptr;
		//int wid = s_w_width, hei = s_w_height;

		s_windowRef = glfwCreateWindow(s_w_width, s_w_height, s_title.c_str(), mon, nullptr);

		if (!s_windowRef)
		{
			s_exitCode = exitCode::fail_GLFW_Window;
			return false;
		}

		glfwMakeContextCurrent(s_windowRef);
		glfwSetWindowSizeLimits(s_windowRef, 320, 180, GLFW_DONT_CARE, GLFW_DONT_CARE);
		//glfwSetWindowAspectRatio(s_windowRef, 16, 9);

		if (!s_fullscreen)
		{
			int monPosX, monPosY, monWidth, monHeight;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
			#ifdef _DEBUG
				// Moves the window to the left of the monitor
				glfwSetWindowPos(s_windowRef, 2, (int)((monHeight - s_w_height) * 0.5f));
				#ifdef _WIN32
					debug::MoveConsole(s_w_width);
				#endif
			#else
				// Moves the window to the center of the workarea
				glfwSetWindowPos(s_windowRef, (int)((monWidth - s_w_width) * 0.5f),
					(int)((monHeight - s_w_height) * 0.5f));
			#endif
		}

		glfwSetFramebufferSizeCallback(s_windowRef, framebufferSizeCallback);

		return true;
	}

	bool application::setupGlad()
	{
		// glad: load all OpenGL function pointers
		if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			return s_gladLoaded = true;

		return false;
	}

	void application::setDimensions(
		const uint16_t _width,
		const uint16_t _height
	) noexcept
	{
		s_w_width = _width;
		s_w_height = _height;

		if (renderer::s_camera && _width > 0 && _height > 0)
			updateCamera();

		//Debug::send(string("Dimensions set to " + to_string(s_w_width) + ", " + to_string(s_w_height)));
	}

	void application::setTitle(const string _title) noexcept
	{
		s_title = _title;
		//debug::send("Title set to \"" + s_title + "\"");
	}

	void application::setFullscreen(const bool _fullscreen) noexcept
	{
		s_fullscreen = _fullscreen;
		//debug::send("Fullscreen set to " + string(s_fullscreen ? "true" : "false"));
	}

	void application::setAppLocation(string _location) noexcept
	{
		size_t last_slash = _location.find_last_of("\\/");
		s_appLocation = _location.substr(0, last_slash + 1);
		//debug::send("App location set to: " + s_appLocation + "\nFrom source: " + _location);
	}

	void application::updateFrameTimeData() noexcept
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
			updateTitle();
		}
	}

	void application::updateCamera() noexcept
	{
		renderer::s_camera->setAspectRatio((float)s_w_width / (float)s_w_height);
		renderer::s_camera->updateFovV();
	}

	void application::updateTitle() noexcept
	{
		string title = {
			s_title
			+ " | "
			+ to_string(s_w_width)
			+ "x"
			+ to_string(s_w_height)
			+ " | "
			+ to_string(s_fps)
		};
		glfwSetWindowTitle(s_windowRef, title.c_str());
	}

	void application::processInput() noexcept
	{
		// TODO: Proper fullscreen toggle
		//if (Input::GetKey(Input::Key::Key_F11, Input::State::Press))
		//{
		//	s_fullscreen = true;
		//	GLFWmonitor *primMon = glfwGetPrimaryMonitor();
		//	int xp, yp;
		//	glfwGetMonitorPos(primMon, &xp, &yp);
		//	const GLFWvidmode *vid = glfwGetVideoMode(primMon);
		//	glfwSetWindowMonitor(s_windowRef, primMon, xp, yp, vid->width, vid->height, vid->refreshRate);
		//}
		//
		//if (Input::GetKey(Input::Key::Key_K, Input::State::Press))
		//{
		//	s_fullscreen = false;
		//	int monPosX, monPosY, monWidth, monHeight;
		//	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
		//	glfwSetWindowMonitor(s_windowRef, nullptr, 2, (int)((monHeight - s_w_height) * 0.5f),
		//		s_w_width, s_w_height, GLFW_DONT_CARE);
		//	glfwSetWindowPos(s_windowRef, 2, (int)((monHeight - s_w_height) * 0.5f));
		//}

		// End application
		if (input::checkKeyState(input::key::Key_End, input::state::Press)) quit();
	}

	void application::framebufferSizeCallback(
		GLFWwindow *_window,
		const int _width,
		const int _height
	) noexcept
	{
		setDimensions((const uint16_t)_width, (const uint16_t)_height);
		renderer::setResolution(_width, _height);
	}

	double application::getTime() noexcept
	{ return s_currentTime; }

	double application::getDeltaTime() noexcept
	{ return s_deltaTime; }

	std::string application::getAppLocation() noexcept
	{ return s_appLocation; }
}
