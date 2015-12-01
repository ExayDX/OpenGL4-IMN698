
#ifndef __OBJ_MODEL_LOADER_H__
#define __OBJ_MODEL_LOADER_H__

#include <gl/glew.h>

#include "ModelLoader.h"

class ObjModelLoader
{
public:
	static ModelContainer* loadModel(
		const std::string, 
		Material* material, 
		GLuint shaderProgram, 
		const std::vector<Vec3>& backgroundColor);
};

#endif //__OBJ_MODEL_LOADER_H__