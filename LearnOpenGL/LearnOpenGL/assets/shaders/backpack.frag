#version 330 core

// I/O
out vec4 FragCol;
in vec2 TexCoords;

struct Material {
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
};

uniform Material u_material;

void main()
{
	FragCol = texture(u_material.texture_diffuse0, TexCoords);
	return;
}