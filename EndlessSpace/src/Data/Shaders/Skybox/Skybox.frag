#version 450 core

layout (location = 0) in vec3 localPos;

layout (location = 0) out vec4 FragColor;
  
layout (binding = 1) uniform samplerCube skybox;
  
void main()
{
    vec3 envColor = texture(skybox, localPos).rgb;
    
    envColor = envColor / (envColor + vec3(1.0));
    envColor = pow(envColor, vec3(1.0/2.2)); 
  
    FragColor = vec4(envColor, 1.0);
}
