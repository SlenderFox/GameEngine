/**
 * @file main.cpp
 * @author SlenderFox
 * @date 2022-11-19
 * @copyright Copyright (c) 2022
 * @brief Following: https://learnopengl.com/
 * @note Disable console through:
 * [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
 * @note Resolutions:
 * 800, 600
 * 1280, 720
 * 1600, 900
 * 1920, 1080
 * @ref Refer to Notes.md to see TODO and notes
 */

#include "Project.hpp"

int main()
{
	Project* app = new Project();
#	ifdef _DEBUG
		app->SetDimensions(1030, 650);
		app->SetTitle("Application (DEBUG)");
#	else
		app->SetDimensions(1600, 900);
		app->SetTitle("Application (RELEASE)");
#	endif
	app->SetFullscreen(false);
	int code = (int)app->Run();
	delete app;
	return code;
}
