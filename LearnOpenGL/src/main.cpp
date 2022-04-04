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
* Maybe add static SetVariable functions in shaderr
*/

#include "Project.hpp"

int main()
{
	Project* app = new Project();
	app->Run(1600, 900, "OpenGL", false);
	delete app;
	return 0;
}
