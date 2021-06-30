#version 330 core

out vec4 FragCol;
in vec4 pixelCol;
void main()
{
    FragCol = pixelCol;
}
