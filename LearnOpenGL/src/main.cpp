/*Following: https://learnopengl.com/
* Enable/disable console through
* [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
* At: https://learnopengl.com/Getting-started/Camera
*/
#include "Application.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

int main()
{
    Application* app = new Application();

    app->Run();

	return 0;
}
