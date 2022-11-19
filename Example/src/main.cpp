/**
 * @file main.cpp
 * @author SlenderFox
 * @brief Following: https://learnopengl.com/
 * Enable/disable console through
 * [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
 * Resolutions:
 * 800, 600
 * 1280, 720
 * 1600, 900
 * 1920, 1080
 * Refer to Obsidian to see TODO and notes
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
