#version 330 core
  
// Vertex Layout
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Scene Matrixes 
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix; 

// Lights information
#define NB_LIGHTS 2
uniform vec3 lightsPositions[NB_LIGHTS]; 

// Return values
out vec3 Normal; 
out vec3 FragPosition;
out vec3 LightsPositions[NB_LIGHTS]; 

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
	Normal = normalMatrix * normal; 

	FragPosition = vec3(view * model * vec4(position, 1.0f));
	for(int i =0; i < NB_LIGHTS; ++i) 
	{
		LightsPositions[i] = vec3(view * vec4(lightsPositions[i], 1.0));
	} 
}