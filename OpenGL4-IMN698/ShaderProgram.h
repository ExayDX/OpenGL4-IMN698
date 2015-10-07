#ifndef __SHADERPROGRAM_H__
#define __SHADERPROGRAM_H__

#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <map>

// Forward Declaration
class FragmentShader; 
class VertexShader; 


//Shader program contains many shaders
class ShaderProgram
{
public:
	ShaderProgram(std::string vertexShaderName, std::string fragmentShaderName);
	GLuint getId(){ return m_programId; }

private:

	GLuint m_programId;
	bool link();
	bool compileShader(GLenum shaderType, const std::string& filename, GLuint& outShaderId); 
};

#endif // __SHADERPROGRAM_H__