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
	ShaderProgram(VertexShader* vs, FragmentShader* fs);

	GLuint getProgramId();
	void insertNewShaderParameterLocation(std::string parameterName, GLuint paremeterLocation); 
	GLuint getShaderParamenterLocation(std::string parameterName); 
	
private:
	GLuint m_programId;
	std::map<std::string, GLuint> m_shaderParametersLocation;
	bool compile(VertexShader* vs, FragmentShader* fs);
	bool link();
};

#endif // __SHADERPROGRAM_H__