

#include "ModelLoader.h"
#include "ObjModelLoader.h"


#include <iostream>


ModelContainer* ModelLoader::loadModel(const std::string filename, Material* material, GLuint shaderProgram)
{
	//check extension and call appropriate fonction
	size_t pos = filename.rfind('.');
	std::string ext;
	if (pos)
		ext = filename.substr(pos + 1);
	else
		assert(false);

	ModelContainer* object = nullptr;

	if (ext == "OBJ" || ext == "obj")
	{
		object = ObjModelLoader::loadModel(filename, material, shaderProgram);
	}
	else
	{
		std::cout << "Format not supported" << std::endl;
	}

	return object;
}