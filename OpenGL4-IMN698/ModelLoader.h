

#ifndef __MODEL_LOADER_H__
#define __MODEL_LOADER_H__

#include <gl/glew.h>

#include "Object.h"

#include <string>

class ModelLoader
{
public:
	static Object* loadModel(const std::string, Material* material , GLuint shaderProgram);
};

#endif //__MODEL_LOADER_H__