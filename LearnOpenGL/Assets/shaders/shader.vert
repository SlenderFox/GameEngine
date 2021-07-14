#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTexCoord;

out vec3 colour;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 camera;

void main()
{
   gl_Position = camera * model * vec4(aPos, 1.0);
   //gl_Position = vec4(aPos, 1.0);
   colour = aCol;
   texCoord = aTexCoord;
}
