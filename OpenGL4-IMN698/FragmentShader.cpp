#include "FragmentShader.h"


FragmentShader::FragmentShader(const std::string& filename) :
Shader(GL_FRAGMENT_SHADER, filename)
{
}

SHADER_TYPE FragmentShader::isA() const
{
	return SHADER_TYPE::FRAGMENT;
}