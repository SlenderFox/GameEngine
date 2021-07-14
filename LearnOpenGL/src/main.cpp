/*Following: https://learnopengl.com/
* Enable/disable console through
* [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
* At: https://learnopengl.com/Getting-started/Camera
* Resolutions:
* 800x600
* 1280x720
* 1600x900
* TODO:
* Get camera fov working
* Create Renderer class
* Convert all classes except Application to singletons
* Make application inheritable
*/
#include "Application.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main()
{
    Application* app = new Application();

    if (app->Start("OpenGL", 1600, 900))
        app->Run();

    delete app;

	return 0;
}
