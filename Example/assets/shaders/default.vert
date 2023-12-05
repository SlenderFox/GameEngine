#version 330 core
layout(location=0)in vec3 aPos;
layout(location=1)in vec3 aNormal;
layout(location=2)in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 u_camera;// Projection*view
uniform mat4 u_model;// Position, rotation, scale
uniform mat3 u_transposeInverseOfModel;

void main()
{
	vec4 vertModel=u_model*vec4(aPos,1.0);
	TexCoords=aTexCoords;
	Normal=normalize(u_transposeInverseOfModel*aNormal);
	FragPos=vertModel.xyz; // Vertex position in world space
	gl_Position=u_camera*vertModel;
}
