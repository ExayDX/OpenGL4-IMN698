
#ifndef __OBJ_MESH_H__
#define __OBJ_MESH_H__

#include "Object.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "Animable.h"

#include <gl/glew.h>

struct vertexStruct
{
	glm::vec3 vertex;
	glm::vec3 normal;
	glm::vec2 uv;

	glm::vec3 surfaceNormal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};


class ObjMesh : public Object
{
public:
	ObjMesh(Material* material,
		GLuint shaderProgram,
		std::vector<vertexStruct> vertices,
		std::vector<double> verticesIndices,
		std::vector<std::string> texturePaths);

	virtual void computeBoundingBox();

	~ObjMesh(){};

	void defineVBO();
	void defineEBO();
	void defineVAO();

	virtual void draw() const;

private:
	std::vector<vertexStruct> m_vertices;
	std::vector<double> m_verticesIndices;

	std::vector<GLuint> m_textures;
};

#endif //__OBJ_MESH_H__