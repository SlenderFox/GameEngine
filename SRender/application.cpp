#include <chrono>
//#include <thread>
#include "application.hpp"
#include "graphics.hpp"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "debug.hpp"

using std::string;
using std::to_string;
using glm::vec3;
using glm::radians;
using glm::normalize;

namespace srender
{
	enum class exitCode: int8_t
	{
		okay = 0,
		fail_GLFW_Init = -1,
		fail_GLFW_Window = -2,
		fail_Glad = -3,
		fail_Renderer = -4,
		fail_Input = -5,
		fail_Startup = -6
	};

	/** Reference to the application */
	application *l_application = nullptr;
	/** Memory is managed by glfw */
	GLFWwindow *l_windowRef = nullptr;
	/** Is the application fullscreen or not */
	bool l_fullscreen = false;
	/** The width and height of the window, should only be modified through setDimensions() */
	uint16_t l_wWidth = 320U, l_wHeight = 180U;
	/** The amount of frames rendered per second */
	uint16_t l_fps = 0U, l_perSecondFrameCount = 0U;
	/** The total amount of frames rendered */
	uint64_t l_totalFrames = 0U;
	/** Used for tracking the time between rendered frames */
	double l_currentTime = 0.0, l_prevTime = 0.0, l_deltaTime = 0.0;
	/** Timers used for calling fixed update and displaying fps */
	double l_fixedTimer = 0.0, l_frameTimer = 0.0;
	/** The title of the window */
	std::string l_title = "SRender";
	/** The location of the application */
	std::string l_appLocation = "";
	/** The error code returned by main */
	exitCode l_exitCode = exitCode::okay;

	void updateFrameTimeData() noexcept
	{
		l_prevTime = l_currentTime;
		l_currentTime = glfwGetTime();
		l_deltaTime = l_currentTime - l_prevTime;
		l_fixedTimer += l_deltaTime;
		l_frameTimer += l_deltaTime;
		++l_totalFrames;
		++l_perSecondFrameCount;

		// Doing this allows me to updates fps as often as I want
		static const double secondsPerUpdate = 0.5;
		if (l_frameTimer >= secondsPerUpdate)
		{
			l_frameTimer -= secondsPerUpdate;
			l_fps = (uint16_t)((double)l_perSecondFrameCount / secondsPerUpdate);
			l_perSecondFrameCount = 0U;
			string title = {
				l_title
				+ " | "
				+ to_string(l_wWidth)
				+ "x"
				+ to_string(l_wHeight)
				+ " | "
				+ to_string(l_fps)
			};
			glfwSetWindowTitle(l_windowRef, title.c_str());
		}
	}

	void updateCamera() noexcept
	{
		renderer::getCamera()->setAspectRatio((float)l_wWidth / (float)l_wHeight);
		renderer::getCamera()->updateFovV();
	}

	void processInput() noexcept
	{
		//if (Input::GetKey(Input::Key::Key_F11, Input::State::Press))
		//{
		//	l_fullscreen = true;
		//	GLFWmonitor *primMon = glfwGetPrimaryMonitor();
		//	int xp, yp;
		//	glfwGetMonitorPos(primMon, &xp, &yp);
		//	const GLFWvidmode *vid = glfwGetVideoMode(primMon);
		//	glfwSetWindowMonitor(l_windowRef, primMon, xp, yp, vid->width, vid->height, vid->refreshRate);
		//}
		//
		//if (Input::GetKey(Input::Key::Key_K, Input::State::Press))
		//{
		//	l_fullscreen = false;
		//	int monPosX, monPosY, monWidth, monHeight;
		//	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
		//	glfwSetWindowMonitor(l_windowRef, nullptr, 2, (int)((monHeight - l_wHeight) * 0.5f),
		//		l_wWidth, l_wHeight, GLFW_DONT_CARE);
		//	glfwSetWindowPos(l_windowRef, 2, (int)((monHeight - l_wHeight) * 0.5f));
		//}

		// End application
		if (input::checkKeyState(input::key::Key_End, input::state::Press))
		{
			application::quit();
		}
	}

	void framebufferSizeCallback(
		GLFWwindow *_window,
		const int _width,
		const int _height
	) noexcept
	{
		application::setDimensions((const uint16_t)_width, (const uint16_t)_height);
		graphics::setResolution(_width, _height);
	}

	_NODISCARD bool setupGLFW()
	{
		// glfw: initialise and configure
		if (!glfwInit())
		{
			l_exitCode = exitCode::fail_GLFW_Init;
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
		GLFWmonitor *monitor = l_fullscreen ? primMon : nullptr;
		const GLFWvidmode *vid = glfwGetVideoMode(primMon);

		glfwWindowHint(GLFW_RED_BITS, vid->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, vid->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, vid->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, vid->refreshRate);

		l_windowRef = glfwCreateWindow(l_wWidth, l_wHeight, l_title.c_str(), monitor, nullptr);

		if (!l_windowRef)
		{
			l_exitCode = exitCode::fail_GLFW_Window;
			return false;
		}

		glfwMakeContextCurrent(l_windowRef);
		glfwSetWindowSizeLimits(l_windowRef, 320, 180, GLFW_DONT_CARE, GLFW_DONT_CARE);
		//glfwSetWindowAspectRatio(l_windowRef, 16, 9);

		// Move the window
		if (!l_fullscreen)
		{
			// TODO: Validate window dimensions do not exceed monitor dimensions
			int monPosX, monPosY, monWidth, monHeight;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);

			#ifdef _DEBUG
				// Moves the window to the left of the monitor
				glfwSetWindowPos(l_windowRef,
					2,
					(int)((monHeight - l_wHeight) * 0.5f)
				);
				#ifdef _WIN32
					debug::moveConsole(l_wWidth);
				#endif
			//#else
				// Moves the window to the center of the workarea
				glfwSetWindowPos(l_windowRef,
					(int)((monWidth - l_wWidth) * 0.5f),
					(int)((monHeight - l_wHeight) * 0.5f)
				);
			#endif
		}

		glfwSetFramebufferSizeCallback(l_windowRef, framebufferSizeCallback);

		return true;
	}

	bool application::init()
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		if (!setupGLFW()) return false;	// Sets own exit code

		if (!graphics::loadGlad())
		{
			l_exitCode = exitCode::fail_Glad;
			return false;
		}

		// Has to be initialised after glfw and glad
		debug::init();

		if (!renderer::init((float)l_wWidth / (float)l_wHeight))
		{
			l_exitCode = exitCode::fail_Renderer;
			return false;
		}

		if (!input::init(l_windowRef))
		{
			l_exitCode = exitCode::fail_Input;
			return false;
		}

		if (!getApplication()->startup())
		{
			l_exitCode = exitCode::fail_Startup;
			return false;
		}

		// Calculates the time it took to start up
		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		debug::send("Started in " + to_string(elapsedTime.count()) + " seconds");

		return true;
	}

	void application::terminate() noexcept
	{
		getApplication()->shutdown();
		renderer::terminate();
		glfwTerminate();
		delete root::getRoot();
	}

	int application::run()
	{
		if (init())
		{
			// Preloads currentTime with an earlier time to prevent first frame weirdness
			l_currentTime = glfwGetTime() - getFixedDeltaTime();

			// Render loop
			while (!(bool)glfwWindowShouldClose(l_windowRef))
			{
				updateFrameTimeData();

				// Input
				glfwPollEvents();
				processInput();
				input::process();

				getApplication()->update();

				// Calls fixed update 60 times per second
				if (l_fixedTimer >= getFixedDeltaTime())
				{
					l_fixedTimer -= getFixedDeltaTime();
					getApplication()->fixedUpdate();
				}

				// Skip drawing if minimised, restricts fps to 15
				if ((bool)glfwGetWindowAttrib(l_windowRef, GLFW_ICONIFIED))
				{
					// TODO: Figure out why sleeping the thread freezes when unminimised
					//std::this_thread::sleep_for(std::chrono::milliseconds(66));

					// Waste time
					double waster = 0.2893652150526226374;
					while (glfwGetTime() < l_currentTime + 0.06666666666666666)
					{
						waster /= waster;
					}
					continue;
				}

				getApplication()->lateUpdate();

				renderer::draw();

				// Check and call events and swap the buffers
				glfwSwapBuffers(l_windowRef);
			}
		}

		terminate();
		return (int)l_exitCode;
	}

	void application::quit() noexcept
	{	glfwSetWindowShouldClose(l_windowRef, true); }

	void application::setDimensions(
		const uint16_t _width,
		const uint16_t _height
	) noexcept
	{
		l_wWidth = _width;
		l_wHeight = _height;

		if (renderer::getCamera() && _width > 0 && _height > 0)
		{	updateCamera(); }

		//debug::send("Dimensions set to " + to_string(l_wWidth) + ", " + to_string(l_wHeight));
	}

	void application::setTitle(const string _title) noexcept
	{
		l_title = _title;
		//debug::send("Title set to \"" + l_title + "\"");
	}

	void application::setFullscreen(const bool _fullscreen) noexcept
	{
		l_fullscreen = _fullscreen;
		//debug::send("Fullscreen set to " + string(l_fullscreen ? "true" : "false"));
	}

	void application::setAppLocation(string _location) noexcept
	{
		size_t last_slash = _location.find_last_of("\\/");
		l_appLocation = _location.substr(0, last_slash + 1);
		//debug::send("App location set to: " + l_appLocation + "\nFrom source: " + _location);
	}

	application *application::getApplication() noexcept
	{	return l_application; }

	double application::getTime() noexcept
	{	return l_currentTime; }

	double application::getDeltaTime() noexcept
	{	return l_deltaTime; }

	std::string application::getAppLocation() noexcept
	{	return l_appLocation; }

	application::application() noexcept
	{
		// Prevents potential memory leak
		assert(!l_application);

		// Applies the static reference
		l_application = this;
	}
}
