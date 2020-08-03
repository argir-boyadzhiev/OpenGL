#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

out vec4 finalColor;

void main()
{
	vec3 Normal = normalize(normalMatrix * aNormal); 
	vec3 FragPos = vec3(view * model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);

	float specularStrength = 0.5;

	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(Normal, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);  

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;  

	vec3 result = (ambient + diffuse + specular) * objectColor;
	finalColor = vec4(result, 1.0);
}