#ifndef __VERTEXSHADER_H__
#define __VERTEXSHADER_H__

#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(const std::string& filename);
	virtual SHADER_TYPE isA() const;
};

#endif //__VERTEXSHADER_H__