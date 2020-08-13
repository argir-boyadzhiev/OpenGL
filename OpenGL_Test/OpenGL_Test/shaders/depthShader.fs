#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec3 ViewPos;

float near = 0.1; 
float far  = 10.0; 

void main()
{   
	float result = (-ViewPos.z - near)/(far - near);
    FragColor = vec4(vec3(result),1.0);
}