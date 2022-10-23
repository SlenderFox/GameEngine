/*Following: https://learnopengl.com/
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
	 int code = (int)app->Run(1030, 650, "OpenGL Application (DEBUG)", false);
#	else
	 int code = (int)app->Run(1600, 900, "OpenGL Application", false);
#	endif
	delete app;
	return code;
}
