#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <map>

#include "Shader.h"
#include "FragmentShader.h"
#include "VertexShader.h"

//Shader program contains many shaders
class ShaderProgram
{
public:
	//ShaderProgram(VertexShader* vs, FragmentShader* fs);
	ShaderProgram();

	GLuint getProgramId();
	void insertNewShaderParameterLocation(std::string parameterName, GLuint paremeterLocation);
	GLuint getShaderParamenterLocation(std::string parameterName);

	void addShader(Shader* shader);
	bool compile();
	bool link();
private:
	GLuint m_programId;
	std::map<std::string, GLuint> m_shaderParametersLocation;

public:
	VertexShader* m_vertexShader;
	FragmentShader* m_fragmentShader;
};

#endif // __SHADERPROGRAM_H__