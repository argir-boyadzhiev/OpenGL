#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec3 ViewPos;

float near = 0.1; 
float far  = 100.0; 

void main()
{   

    FragColor = vec4(0,0,0,1.0);
}