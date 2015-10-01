#ifndef __VERTEXSHADER_H__
#define __VERTEXSHADER_H__

#include "Shader.h"

class VertexShader : public Shader
{
public:
	VertexShader(const std::string& filename);
};

#endif //__VERTEXSHADER_H__