

#include "ObjModelLoader.h"
#include "ObjMesh.h"

#include "SOIL/SOIL.h"

#include <fstream>
#include <iostream>
#include <vector>

bool isVertex(const std::string& line)
{
	return line[0] == 'v' && line[1] == ' ';
}

void addVertex(std::vector<glm::vec3>& vertices, const std::string& line)
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

	vertices.push_back(vertex);
}

bool isUv(const std::string& line)
{
	return line[0] == 'v' && line[1] == 't' && line[2] == ' ';
}

void addUv(std::vector<glm::vec2>& uvs, const std::string& line)
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

	uvs.push_back(uv);
}

bool isNormal(const std::string& line)
{
	return line[0] == 'v' && line[1] == 'n' && line[2] == ' ';
}

void addNormal(std::vector<glm::vec3>& normals, const std::string& line)
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

	normals.push_back(normal);
}

bool isFace(const std::string& line)
{
	return line[0] == 'f' && line[1] == ' ';
}

void addFace(
	std::vector<double>& verticesIndices, 
	std::vector<double>& uvsIndices, 
	std::vector<double>& normalsIndices,
	std::vector<int>& vertexPerFaces,
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
		

		verticesIndices.push_back(atof(vertexIndex.c_str()));
		if (!uvIndex.empty())
			uvsIndices.push_back(atof(uvIndex.c_str()));
		if (!normalIndex.empty())
			normalsIndices.push_back(atof(normalIndex.c_str()));

		data = data.substr(nextDataPos + 1);
		finished = nextDataPos == std::string::npos;
	}

	vertexPerFaces.push_back(vertexPerFace);
}

bool isTexturePath(const std::string& line)
{
	return line.substr(0,7) == "texture";
}

void addTexturePath(
	std::vector<std::string>& texturePaths,
	const std::string& line)
{
	std::string texturePath = line.substr(8);
	texturePaths.push_back(texturePath);
}

//-----------------------------------------------------------------------------
// Load an .obj file
//-----------------------------------------------------------------------------
Object* ObjModelLoader::loadModel(const std::string filename, Material* material, GLuint shaderProgram)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		throw std::runtime_error("could not create shader: file can't be opened");
		return nullptr;
	}

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
			addVertex(vertices, line);
		else if (isUv(line))
			addUv(uvs, line);
		else if (isNormal(line))
			addNormal(normals, line);
		else if (isFace(line))
			addFace(verticesIndices, uvsIndices, normalsIndices, vertexPerFaces, line);
		else if (isTexturePath(line))
			addTexturePath(texturePaths, line);

	}

	std::vector<vertexStruct> verticesStructs;

	//create vertex object for each vertex in each face
	int currentIndex = 0;
	for (int i = 0; i < vertexPerFaces.size(); i++)
	{
		int vertexPerPolygon = vertexPerFaces[i];
		assert(vertexPerPolygon == 3); //For now we only support triangulated meshes

		for (int j = 0; j < vertexPerPolygon; j++)
		{
			vertexStruct vert;
			vert.vertex = vertices[verticesIndices[currentIndex + j] - 1];

			if (!normals.empty())
			{
				vert.normal = glm::normalize(normals[normalsIndices[currentIndex + j] - 1]);
				vert.tangent = glm::normalize(glm::vec3(-vert.normal.y, vert.normal.x, vert.normal.z));
				vert.bitangent = glm::normalize(glm::cross(vert.tangent, vert.normal));
			}
			if (!uvs.empty())
			{
				vert.uv = uvs[uvsIndices[currentIndex + j] - 1];
			}
			verticesStructs.push_back(vert);

		}

		currentIndex += vertexPerPolygon;
	}

	return new ObjMesh(material,
					shaderProgram,
					verticesStructs,
					verticesIndices,
					texturePaths
					);
}