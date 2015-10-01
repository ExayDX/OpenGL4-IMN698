#include "ShaderProgram.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include <cassert>

ShaderProgram::ShaderProgram(VertexShader* vs, FragmentShader* fs) :
m_programId(glCreateProgram())
{
	assert(vs, "Attempted to create a Shader program without vertex shader (vs = nullptr)");
	assert(fs, "Attempted to create a Shader program without fragment shader (fs = nullptr)");

	if (!compile(vs, fs))
	{
		// TODO : MANAGE ERROR;
	}

	//Attach shaders
	glAttachShader(m_programId, vs->getShaderId());
	glAttachShader(m_programId, fs->getShaderId());

	if (!link())
	{
		// TODO : MANAGE ERROR
	}

	glDeleteShader(vs->getShaderId());
	glDeleteShader(fs->getShaderId());

}


bool ShaderProgram::link()
{
	GLint isLinked = GL_FALSE;

	glLinkProgram(m_programId);
	glGetProgramiv(m_programId, GL_LINK_STATUS, &isLinked);

	GLchar infoLog[512];
	if (!isLinked)
	{
		glGetProgramInfoLog(m_programId, 512, nullptr, infoLog);
		std::string errorMessage = "ERROR::SHADER::PROGRAM::LINK_FAILED\n";
		errorMessage.append(infoLog).append("\n");
		throw std::runtime_error(errorMessage);
	}

	return isLinked; 

}

bool ShaderProgram::compile(VertexShader* vs, FragmentShader* fs)
{
	bool result = true;

	if (vs)
		result = result && vs->compile();
	else
		result = false;

	if (fs)
		result = result && fs->compile();
	else
		result = false;

	return result;
}


GLuint ShaderProgram::getProgramId()
{
	return m_programId;
}

void ShaderProgram::insertNewShaderParameterLocation(std::string parameterName, GLuint paremeterLocation)
{
	m_shaderParametersLocation.insert(std::pair<std::string, GLuint>(parameterName, paremeterLocation)); 
}

GLuint ShaderProgram::getShaderParamenterLocation(std::string parameterName)
{
	return m_shaderParametersLocation[parameterName]; 
}