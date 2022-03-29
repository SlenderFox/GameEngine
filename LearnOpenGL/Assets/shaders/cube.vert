#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 u_camera; // Projection * view
uniform mat4 u_model;
uniform mat3 u_transposeInverseOfModel;

void main()
{
   //gl_Position = vec4(aPos, 1.0);
   gl_Position = u_camera * u_model * vec4(aPos, 1.0);
   FragPos = vec3(u_model * vec4(aPos, 1.0));  // Vertex position in world space
   Normal = u_transposeInverseOfModel * aNormal;
   TexCoord = aTexCoord;
}
