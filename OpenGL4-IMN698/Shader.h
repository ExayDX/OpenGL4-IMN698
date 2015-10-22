#ifndef __SHADER_H__
#define __SHADER_H__

#include <gl/glew.h>
#include "GLfW/glfw3.h"

#include <string>

enum SHADER_TYPE
{
	FRAGMENT,
	VERTEX
};

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

	virtual SHADER_TYPE isA() const = 0;

protected:
	Shader(GLenum type, const std::string& filename);
	Shader(GLenum type);
};


#endif //__SHADER_H__