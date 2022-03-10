#version 330 core
out vec4 FragCol;

//in vec3 colour;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragCol = mix(mix(texture(texture0, texCoord), texture(texture1, texCoord), 0.6), texture(texture2, texCoord), 0.1);
}
