#version 330 core
out vec4 FragCol;

uniform vec3 u_colour;

void main()
{
	FragCol = vec4(u_colour, 1.0);
}
