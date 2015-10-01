#ifndef __SHADER_H__
#define __SHADER_H__

#include <gl/glew.h>
#include "GLfW/glfw3.h"

#include <string>


class Shader
{

private:
	std::string m_code;

	GLenum m_type;
	GLuint m_shaderId;

public:
	bool compile();

	void setCode(const std::string& code);

	GLuint getShaderId();

protected:
	Shader(GLenum type, const std::string& filename);
	Shader(GLenum type);
};


#endif //__SHADER_H__