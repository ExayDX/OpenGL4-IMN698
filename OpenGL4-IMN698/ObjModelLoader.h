
#ifndef __OBJ_MODEL_LOADER_H__
#define __OBJ_MODEL_LOADER_H__


#include "ModelLoader.h"

class ObjModelLoader
{
public:
	static Object* loadModel(const std::string, Material* material, GLuint shaderProgram);
};

#endif //__OBJ_MODEL_LOADER_H__