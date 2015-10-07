

#include <cassert>

#include "ShaderProgram.h"
#include "FragmentShader.h"
#include "VertexShader.h"
#include "Shader.h"

ShaderProgram::ShaderProgram() :
	m_programId(glCreateProgram())
{

}

void ShaderProgram::addShader(Shader* shader)
{
	//according to shader type, assign to according variable
	VertexShader* vs = dynamic_cast<VertexShader*>(shader);
	FragmentShader* fs = dynamic_cast<FragmentShader*>(shader);
	if (vs)
	{
		m_vertexShader = vs;
	}
	else if (fs)
	{
		m_fragmentShader = fs;
	}
}

bool ShaderProgram::link()
{
	GLint isLinked = GL_FALSE;
	
	if (m_vertexShader)
		glAttachShader(m_programId, m_vertexShader->getShaderId());
	if (m_fragmentShader)
		glAttachShader(m_programId, m_fragmentShader->getShaderId());

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

bool ShaderProgram::compile()
{
	bool result = true;

	if (m_vertexShader)
		result = result && m_vertexShader->compile();
	else
		result = false;

	if (m_fragmentShader)
		result = result && m_fragmentShader->compile();
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