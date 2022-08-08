#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 u_camera; // Projection * view
uniform mat4 u_model;

void main()
{
   gl_Position = u_camera * u_model * vec4(aPos, 1.0);
   TexCoords = aTexCoords;
}
