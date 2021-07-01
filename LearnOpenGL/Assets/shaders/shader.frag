#version 330 core
out vec4 FragCol;

in vec3 colour;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    FragCol = mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.2);
}
