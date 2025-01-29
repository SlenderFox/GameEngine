# SRender
Barely functional, bug-ridden mess.
Project originally started with me following https://learnopengl.com/

# Compiling
Can be compiled on Windows with Visual Studio (msvc).
Can be compiled on Linux with makefile (gcc).

## Libraries used:
- OpenGL core 3.3 via glad ([Website](https://glad.dav1d.de/))
- GLFW ([Website](https://www.glfw.org/download.html))
- GLM ([Website](https://glm.g-truc.net/0.9.8/index.html))
- stb_image.h ([Github](https://github.com/nothings/stb/blob/master/stb_image.h))
- Assimp ([Github](https://github.com/assimp/assimp))

## Notes
Classes are used when inheritance is intended, structs are for when it is not.
Prefer composition over inheritance.

## Ideas
- When application launches, don't steal focus and mouse.
- Instead of window automatically launching, project can request to create a window and then apply callbacks to the window.
- Make things more static.
