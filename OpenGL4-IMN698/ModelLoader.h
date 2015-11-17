

#ifndef __MODEL_LOADER_H__
#define __MODEL_LOADER_H__

#include <gl/glew.h>

#include "Object.h"
#include "ModelContainer.h"
#include <string>

class ModelLoader
{
public:
	static ModelContainer* loadModel(
		const std::string, 
		Material* material, 
		GLuint shaderProgram, 
		const std::vector<Vec3>& backgroundColor);
};

#endif //__MODEL_LOADER_H__