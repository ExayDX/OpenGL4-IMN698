#include "VertexShader.h"


VertexShader::VertexShader(const std::string& filename) :
Shader(GL_VERTEX_SHADER, filename)
{

}

SHADER_TYPE VertexShader::isA() const
{
	return SHADER_TYPE::VERTEX;
}