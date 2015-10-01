#ifndef __FRAGMENTSHADER_H__
#define __FRAGMENTSHADER_H__

#include "Shader.h"
#include <string>

class FragmentShader : public Shader
{
public:
	FragmentShader(const std::string& filename);

};

#endif //__FRAGMENTSHADER_H__