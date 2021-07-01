/*Following: https://learnopengl.com/
* Enable/disable console through
* [Properties>Linker>System>Subsystem & Properties>Linker>Advanced>Entry Point (mainCRTStartup)]
* At: https://learnopengl.com/Getting-started/Coordinate-Systems
*/
#include "Application.h"
#include <iostream>

#define WAITFORINPUT true;

using std::cout;
using std::cin;
using std::endl;

int main()
{
    Application* app = new Application();

    if (!app->run())
    {
        // Once the window has closed keep the console open
#ifdef WAITFORINPUT
        // Waits for any input
        cout << "Press enter to close...";
        cin.clear();
        cin.ignore(cin.rdbuf()->in_avail());
        cin.get();
#else
        // Waits 5 seconds
        //int closeTime = glfwGetTime() + 5;
        //while (glfwGetTime() < closeTime) { }
#endif
    }

	return 0;
}
