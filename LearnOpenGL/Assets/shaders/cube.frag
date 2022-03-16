#version 330 core
out vec4 FragCol;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec3 ambient;
uniform vec3 lightPos;
uniform vec3 lightCol;
uniform vec3 viewPos;

void main()
{
    // Texture blending
    vec4 texCol = mix(mix(texture(texture0, TexCoord), texture(texture1, TexCoord), 0.6), texture(texture2, TexCoord), 0.1);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // Diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightCol;
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightCol;

    vec3 phong = (ambient + diffuse + specular);

    FragCol = texCol * vec4(phong, 1);
    //FragCol = vec4(phong, 1.0);
}
