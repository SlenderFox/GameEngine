/*Following: https://learnopengl.com/
* Enable/disable console through
* [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
* At: https://learnopengl.com/Model-Loading/Model
* Resolutions:
* 800, 600
* 1280, 720
* 1600, 900
* 1920, 1080
* TODO:
* Fix spotlight not adjusting in real time
* Maybe add static SetVariable functions in shader
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
