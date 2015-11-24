
#include "ObjModelLoader.h"
#include "ModelContainer.h"

#include "SOIL/SOIL.h"

#include <fstream>
#include <iostream>
#include <vector>

class ModelContainer;

bool isVertex(const std::string& line)
{
	return line[0] == 'v' && line[1] == ' ';
}

void addVertex(ModelContainer* model, const std::string& line)
{
	glm::vec3 vertex;
	size_t firstBlank = line.find_first_of(' ');
	std::string subLine = line.substr(firstBlank + 1);
	
	size_t secondBlank = subLine.find_first_of(' ');
	std::string x = subLine.substr(0, secondBlank);
	vertex.x = atof(x.c_str());
	subLine = subLine.substr(secondBlank + 1);
	
	size_t thirdBlank = subLine.find_first_of(' ');
	std::string y = subLine.substr(0, thirdBlank);
	vertex.y = atof(y.c_str());
	subLine = subLine.substr(thirdBlank + 1);

	size_t fourthBlank = subLine.find_first_of(' ');
	std::string z = subLine.substr(0, fourthBlank);
	vertex.z = atof(z.c_str());
	subLine = subLine.substr(fourthBlank + 1);

	model->addVertex(vertex);
}

bool isUv(const std::string& line)
{
	return line[0] == 'v' && line[1] == 't' && line[2] == ' ';
}

void addUv(ModelContainer* model, const std::string& line)
{
	glm::vec2 uv;
	size_t firstBlank = line.find_first_of(' ');
	std::string subLine = line.substr(firstBlank + 1);

	size_t secondBlank = subLine.find_first_of(' ');
	std::string x = subLine.substr(0, secondBlank);
	uv.x = atof(x.c_str());
	subLine = subLine.substr(secondBlank + 1);

	size_t thirdBlank = subLine.find_first_of(' ');
	std::string y = subLine.substr(0, thirdBlank);
	uv.y = atof(y.c_str());
	subLine = subLine.substr(thirdBlank + 1);

	model->addUv(uv);
}

bool isNormal(const std::string& line)
{
	return line[0] == 'v' && line[1] == 'n' && line[2] == ' ';
}

void addNormal(ModelContainer* model, const std::string& line)
{
	glm::vec3 normal;
	size_t firstBlank = line.find_first_of(' ');
	std::string subLine = line.substr(firstBlank + 1);

	size_t secondBlank = subLine.find_first_of(' ');
	std::string x = subLine.substr(0, secondBlank);
	normal.x = atof(x.c_str());
	subLine = subLine.substr(secondBlank + 1);

	size_t thirdBlank = subLine.find_first_of(' ');
	std::string y = subLine.substr(0, thirdBlank);
	normal.y = atof(y.c_str());
	subLine = subLine.substr(thirdBlank + 1);

	size_t fourthBlank = subLine.find_first_of(' ');
	std::string z = subLine.substr(0, fourthBlank);
	normal.z  = atof(z.c_str());
	subLine = subLine.substr(fourthBlank + 1);

	model->addNormal(normal);
}

bool isFace(const std::string& line)
{
	return line[0] == 'f' && line[1] == ' ';
}

void addFace(
	ModelContainer* model,
	const std::string& line)
{
	int vertexPerFace=0;

	size_t firstBlank = line.find_first_of(' ');
	std::string data = line.substr(firstBlank + 1);

	bool finished = false;
	while (!finished)
	{
		vertexPerFace += 1;
		std::string vertexIndex;
		std::string uvIndex;
		std::string normalIndex;

		size_t nextDataPos = data.find_first_of(' ');
		std::string currentData = data.substr(0, nextDataPos);

		size_t firstSlash = data.find_first_of('/');
		vertexIndex = currentData.substr(0, firstSlash);

		currentData = currentData.substr(firstSlash + 1);
		size_t secondSlash = currentData.find_first_of('/');
		if (secondSlash != std::string::npos)
		{
			uvIndex = currentData.substr(0, secondSlash);
			normalIndex = currentData.substr(secondSlash + 1);
		}
		else //no normal
		{
			uvIndex = currentData;
		}
		
		model->addVertexIndex(atof(vertexIndex.c_str()));
		if (!uvIndex.empty())
			model->addUvIndex(atof(uvIndex.c_str()));
		if (!normalIndex.empty())
			model->addNormalIndex(atof(normalIndex.c_str()));

		data = data.substr(nextDataPos + 1);
		finished = nextDataPos == std::string::npos;
	}

	model->addVertexPerFace(vertexPerFace);
}

bool isTexturePath(const std::string& line)
{
	return line.substr(0,7) == "texture";
}

void addTexturePath(
	ModelContainer* model,
	const std::string& line)
{
	std::string texturePath = line.substr(8);
	model->addTexturePath(texturePath);
}

//-----------------------------------------------------------------------------
// Load an .obj file
//-----------------------------------------------------------------------------
ModelContainer* ObjModelLoader::loadModel(const std::string filename, Material* material, GLuint shaderProgram)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		throw std::runtime_error("could not create model: file can't be opened");
		return nullptr;
	}

	ModelContainer* model = new ModelContainer(material, shaderProgram);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<double> verticesIndices;
	std::vector<double> uvsIndices;
	std::vector<double> normalsIndices;

	std::vector<int> vertexPerFaces;
	std::vector<std::string> texturePaths;

	//std::vector<double> normals;

	std::string line;
	while (getline(file, line))
	{
		if (isVertex(line))
			addVertex(model, line);
		else if (isUv(line))
			addUv(model, line);
		else if (isNormal(line))
			addNormal(model, line);
		else if (isFace(line))
			addFace(model, line);
		else if (isTexturePath(line))
			addTexturePath(model, line);

	}
	model->finish();
	return model;
}