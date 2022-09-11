#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 u_camera;
uniform mat4 u_model;
uniform vec3 u_scale = vec3(1);

void main()
{
   // Makes the light a smaller cube
   gl_Position = u_camera * u_model * (vec4(aPos, 1.0) * vec4(u_scale, 1));
}
