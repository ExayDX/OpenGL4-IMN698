#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

layout (location = 3) in vec3 surfaceNormal;
layout (location = 4) in vec3 tangent;
layout (location = 5) in vec3 bitangent;

#define NB_LIGHTS 2

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;
uniform vec3 lightsPositions[NB_LIGHTS];
uniform vec3 cameraPos;

out vec3 Normal;
out vec3 LightsPositions[NB_LIGHTS];


out VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

void main()
{
  gl_Position = projection * view * model * vec4(position, 1.0f);
  vs_out.FragPos = vec3(model * vec4(position, 1.0));
  vs_out.TexCoords = vec2(texCoords.x, 1 - texCoords.y);
  //default normal
  Normal = normalMatrix * normal;

  mat3 normalMatrix = transpose(inverse(mat3(model)));
  vec3 T = normalize(normalMatrix * tangent);
  vec3 B = normalize(normalMatrix * bitangent);
  vec3 N = normalize(normalMatrix * normal);

  mat3 TBN = transpose(mat3(T, B, N));
  vs_out.TangentLightPos = TBN * lightsPositions[0];
  vs_out.TangentViewPos  = TBN * cameraPos;
  vs_out.TangentFragPos  = TBN * vs_out.FragPos;

	for(int i =0; i < NB_LIGHTS; ++i)
	{
		LightsPositions[i] = vec3(view * vec4(lightsPositions[i], 1.0));
	}
}
