#version 330 core
out vec4 FragColor;

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
in vec3 ViewPos;

void main()
{   
    FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}