#include <chrono>
//#include <thread>
#include "application.hpp"
#include "renderer.hpp"
#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "debug.hpp"

using std::string;
using std::to_string;

namespace srender
{
namespace application
{
	enum class exitCode: int8_t
	{
		okay = 0,
		fail_GLFW_Init = -1,
		fail_GLFW_Window = -2,
		fail_Glad = -3,
		fail_Renderer = -4,
		fail_Input = -5
		//fail_Startup = -6
	};

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
	/** Called once at the start of the runtime */
	callbackFunc l_startupCallback = nullptr;
	/** Called when the application shuts down */
	callbackFunc l_shutdownCallback = nullptr;
	/** Called once at the start of every frame */
	callbackFunc l_updateCallback = nullptr;
	/** Called 60 times per second, after Update */
	callbackFunc l_fixedUpdateCallback = nullptr;
	/** Called once per frame, after fixedUpdate but still before rendering */
	callbackFunc l_lateUpdateCallback = nullptr;

	constexpr double titleUpdateInterval() { return 0.5; }

	inline void updateCamera() noexcept
	{
		graphics::getCamera()->setAspectRatio((float)l_wWidth / (float)l_wHeight);
		graphics::getCamera()->updateFovV();
	}

	inline void processInput() noexcept
	{
		//if (Input::GetKey(input::key::key_f11, input::state::press))
		//{
		//	l_fullscreen = true;
		//	GLFWmonitor *primMon = glfwGetPrimaryMonitor();
		//	int xp, yp;
		//	glfwGetMonitorPos(primMon, &xp, &yp);
		//	const GLFWvidmode *vid = glfwGetVideoMode(primMon);
		//	glfwSetWindowMonitor(l_windowRef, primMon, xp, yp, vid->width, vid->height, vid->refreshRate);
		//}
		//
		//if (Input::GetKey(input::key::key_k, input::state::press))
		//{
		//	l_fullscreen = false;
		//	int monPosX, monPosY, monWidth, monHeight;
		//	glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);
		//	glfwSetWindowMonitor(l_windowRef, nullptr, 2, (int)((monHeight - l_wHeight) * 0.5f),
		//		l_wWidth, l_wHeight, GLFW_DONT_CARE);
		//	glfwSetWindowPos(l_windowRef, 2, (int)((monHeight - l_wHeight) * 0.5f));
		//}

		// End application
		if (input::checkKeyState(input::key::key_end, input::state::press))
		{
			quit();
		}
	}

	void framebufferSizeCallback(
		GLFWwindow *_window,
		const int _width,
		const int _height
	) noexcept
	{
		setDimensions((const uint16_t)_width, (const uint16_t)_height);
		renderer::setResolution(_width, _height);
	}

	_NODISCARD inline bool setupGLFW()
	{
		// glfw: initialise and configure
		//> Something in here causes output to work with wchar
		if (!glfwInit())
		{
			l_exitCode = exitCode::fail_GLFW_Init;
			const char *description;
			int code = glfwGetError(&description);
			assert(description != nullptr);
			debug::send(
				"Code: "
				+ std::to_string(code)
				+ "\nDescription: "
				+ description
				+ "\n"
			);
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

		l_windowRef = glfwCreateWindow(
			l_wWidth,
			l_wHeight,
			l_title.c_str(),
			monitor,
			nullptr
		);

		if (!l_windowRef)
		{
			l_exitCode = exitCode::fail_GLFW_Window;
			const char *description;
			int code = glfwGetError(&description);
			assert(description != nullptr);
			debug::send(
				"Code: "
				+ std::to_string(code)
				+ "\nDescription: "
				+ description
				+ "\n"
			);
			return false;
		}

		glfwMakeContextCurrent(l_windowRef);
		glfwSetWindowSizeLimits(l_windowRef, 320, 180, GLFW_DONT_CARE, GLFW_DONT_CARE);
		//glfwSetWindowAspectRatio(l_windowRef, 16, 9);
		// Disable v-sync
		glfwSwapInterval(0);

		// Move the window
		if (!l_fullscreen)
		{
			// TODO: Validate window dimensions do not exceed monitor dimensions
			int monPosX, monPosY, monWidth, monHeight;
			glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &monPosX, &monPosY, &monWidth, &monHeight);

			#ifdef _DEBUG
				#ifdef _WIN32
					// Moves the window to the left of the monitor
					glfwSetWindowPos(l_windowRef,
						2,
						(int)((monHeight - l_wHeight) * 0.5f)
					);
					// And the console to the right
					debug::moveConsole(l_wWidth);
				#else
					// Moves the window to the center of the workarea, slightly raised
					glfwSetWindowPos(l_windowRef,
						(int)((monWidth - l_wWidth) * 0.5f),
						31
					);
				#endif
			#else
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

	_NODISCARD inline bool init()
	{
		auto startTime = std::chrono::high_resolution_clock::now();

		if (!setupGLFW()) return false;	// Sets own exit code

		if (!renderer::loadGlad())
		{
			l_exitCode = exitCode::fail_Glad;
			return false;
		}

		if (!graphics::init((float)l_wWidth / (float)l_wHeight))
		{
			l_exitCode = exitCode::fail_Renderer;
			return false;
		}

		if (!input::init(l_windowRef))
		{
			l_exitCode = exitCode::fail_Input;
			return false;
		}

		if (l_startupCallback)
		{ l_startupCallback(); }
		//{
		//	l_exitCode = exitCode::fail_Startup;
		//	return false;
		//}

		// Calculates the time it took to start up
		auto endTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsedTime = endTime - startTime;
		debug::send("Initialised in " + to_string(elapsedTime.count()) + " seconds");

		return true;
	}

	/**
	 * Order is:
	 * Process Input > Update > Fixed Update > Late Update > Draw
	 */
	inline void loop()
	{
		while (!(bool)glfwWindowShouldClose(l_windowRef))
		{
			auto updateStart = std::chrono::high_resolution_clock::now();

			// TODO: Look into extracting into a timekeeping class
			l_prevTime = l_currentTime;
			l_currentTime = glfwGetTime();
			l_deltaTime = l_currentTime - l_prevTime;
			l_fixedTimer += l_deltaTime;
			l_frameTimer += l_deltaTime;
			++l_totalFrames;
			++l_perSecondFrameCount;

			// Input
			glfwPollEvents();
			processInput();
			input::process();

			if (l_updateCallback)
			{ l_updateCallback(); }

			// Calls fixed update 60 times per second
			if (l_fixedTimer >= getFixedDeltaTime())
			{
				l_fixedTimer -= getFixedDeltaTime();
				if (l_fixedUpdateCallback)
				{ l_fixedUpdateCallback(); }
			}

			if (l_lateUpdateCallback)
			{ l_lateUpdateCallback(); }

			// Skip drawing if minimised, restricts fps to 15
			if ((bool)glfwGetWindowAttrib(l_windowRef, GLFW_ICONIFIED))
			{
				// TODO: Figure out why sleeping the thread freezes when unminimised
				// Probably doesn't realised it has unminimised lol
				//std::this_thread::sleep_for(std::chrono::milliseconds(66));

				// Waste time
				double waster = 0.2893652150526226374;
				while (glfwGetTime() < l_currentTime + 0.06666666666666666)
				{	waster /= waster; }
				continue;
			}

			auto frameStart = std::chrono::high_resolution_clock::now();
			graphics::draw();

			// Check and call events and swap the buffers
			glfwSwapBuffers(l_windowRef);

			auto updateEnd = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double> updateTime = updateEnd - updateStart;
			std::chrono::duration<double> frameTime = updateEnd - frameStart;

			// Doing this allows me to updates fps as often as I want
			if (l_frameTimer >= titleUpdateInterval())
			{
				l_frameTimer -= titleUpdateInterval();
				l_fps = (uint16_t)((double)l_perSecondFrameCount / titleUpdateInterval());
				l_perSecondFrameCount = 0U;
				string updateTimeMs = to_string(updateTime.count() * 1000);
				updateTimeMs.resize(5);
				string frameTimeMs = to_string(frameTime.count() * 1000);
				frameTimeMs.resize(5);
				string title = {
					l_title
					+ " | "
					+ to_string(l_wWidth)
					+ "x"
					+ to_string(l_wHeight)
					+ " | FPS: "
					+ to_string(l_fps)
					+ ", FT/UT: "
					+ frameTimeMs
					+ "/"
					+ updateTimeMs
				};
				glfwSetWindowTitle(l_windowRef, title.c_str());
			}
		}
	}

	inline void terminate() noexcept
	{
		if (l_shutdownCallback)
		{ l_shutdownCallback(); }
		graphics::terminate();
		glfwTerminate();
	}

	inline void setAppLocation(std::string _path) noexcept
	{
		size_t last_slash = _path.find_last_of("\\/");
		l_appLocation = _path.substr(0, last_slash + 1);
		//> can uncomment when fixed wchar issue
		//#ifdef _VERBOSE
		//	debug::send("App location set to: " + l_appLocation + "\nFrom source: " + _path);
		//#endif
	}

	void quit() noexcept
	{	glfwSetWindowShouldClose(l_windowRef, true); }

	void setDimensions(
		const uint16_t _width,
		const uint16_t _height
	) noexcept
	{
		l_wWidth = _width;
		l_wHeight = _height;

		if (graphics::getCamera() && _width > 0 && _height > 0)
		{	updateCamera(); }
	}

	void setTitle(const string _title) noexcept
	{	l_title = _title; }

	void setFullscreen(const bool _fullscreen) noexcept
	{	l_fullscreen = _fullscreen; }

	void setStartupCallback(callbackFunc _func) noexcept
	{ l_startupCallback = _func; }

	void setShutdownCallback(callbackFunc _func) noexcept
	{ l_shutdownCallback = _func; }

	void setUpdateCallback(callbackFunc _func) noexcept
	{ l_updateCallback = _func; }

	void setFixedUpdateCallback(callbackFunc _func) noexcept
	{ l_fixedUpdateCallback = _func; }

	void setLateUpdateCallback(callbackFunc _func) noexcept
	{ l_lateUpdateCallback = _func; }

	double getTime() noexcept
	{	return l_currentTime; }

	double getDeltaTime() noexcept
	{	return l_deltaTime; }

	std::string getAppLocation() noexcept
	{	return l_appLocation; }

	int run(int _argc, char *_args[])
	{
		debug::init();
		//> can uncomment when fixed wchar issue
		//debug::send("Initialising...");
		setAppLocation(_args[0]);

		if (init())
		{
			// Preloads currentTime with an earlier time to prevent first frame weirdness
			l_currentTime = glfwGetTime() - getFixedDeltaTime();
			l_frameTimer = titleUpdateInterval();

			// Update-Render loop
			loop();
		}

		terminate();
		return (int)l_exitCode;
	}
}
}
