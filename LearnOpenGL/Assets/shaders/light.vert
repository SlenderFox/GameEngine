#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

out vec3 Colour;

uniform mat4 camera;
uniform mat4 model;

void main()
{
   gl_Position = camera * model * vec4(aPos, 1.0);
   Colour = aCol;
}
