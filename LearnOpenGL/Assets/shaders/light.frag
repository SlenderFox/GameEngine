#version 330 core
out vec4 FragCol;

uniform vec3 colour;

void main()
{
    FragCol = vec4(colour, 1.0);
}
