#version 330 core

// Vertex Layout
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

// Scene Matrixes
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

// Lights information
#define NB_LIGHTS 2
uniform vec3 lightsPositions[NB_LIGHTS];

// Return values
out VS_OUT {
    vec2 TexCoords;
    vec3 TBNViewPosition;
	  vec3 FragPosition;
	  vec3 LightsPositions[NB_LIGHTS];
} vs_out;

void main()
{
	// Mesh's vertex position
	gl_Position = projection * view * model * vec4(position, 1.0f);

	// Construction of the TBN matrix
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);
	mat3 TBN = transpose(mat3(T, B, N));

	// Return values
	vs_out.TexCoords = vec2(texCoords.x, 1 - texCoords.y);
	vs_out.TBNViewPosition = TBN * vec3(0.0f, 0.0f, 0.0f); // Camera is at 0.0f,0.0f,0.0f; 
	vs_out.FragPosition = TBN * vec3( view *  model * vec4(position, 1.0));
	
	for(int i =0; i < NB_LIGHTS; ++i)
	{
		vs_out.LightsPositions[i] = TBN * vec3(view * vec4(lightsPositions[i], 1.0));
	}
}
