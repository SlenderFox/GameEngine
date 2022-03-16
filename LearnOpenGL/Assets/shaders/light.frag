#version 330 core
out vec4 FragCol;

in vec3 Colour;

void main()
{
    FragCol = vec4(Colour, 1.0);
}
