/*Following: https://learnopengl.com/
* Currently at: https://learnopengl.com/Model-Loading/Model
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
	#ifdef _DEBUG
	 app->Run(1030, 650, "OpenGL Application (_DEBUG)", false);
	#else
	 app->Run(1600, 900, "OpenGL Application", false);
	#endif
	delete app;
	return 0;
}
