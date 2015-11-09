
#ifndef __MODEL_CONTAINER_H__
#define __MODEL_CONTAINER_H__

#include "Object.h"

#include "glm/glm/glm.hpp"

#include <vector>

class Object;

// container of a 3d model
class ModelContainer: public Object
{
	typedef glm::vec3			Vertex;
	typedef glm::vec3			Normal;
	typedef glm::vec2			Uv;
	typedef glm::vec3			Tangent;
	typedef glm::vec3			Bitangent;
	typedef std::vector<int>	Face; //We support any face number
	typedef int					FaceIndex;

public:
	struct VertexData
	{
		Vertex		m_vertex;
		Normal		m_normal;
		Uv			m_uv;
		Tangent		m_tangent;
		Bitangent	m_bitangent;

		std::vector<FaceIndex> m_faces;
	};


public:
	//Object member functions
	void defineVBO();
	void defineEBO();
	void defineVAO();

	virtual void draw();

public:
	ModelContainer(Material* material, GLuint shaderProgram);

	void addVertex(Vertex vertex);
	void addNormal(Normal normal);
	void addUv(Uv uv);

	void addVertexIndex(int index);
	void addNormalIndex(int index);
	void addUvIndex(int index);
	void addVertexPerFace(int num);


	void addTexturePath(std::string path);

	void triangulate();
	void getTriangles();

	void computeNormals();
	void smoothNormals();

	void computeTangents();
	void computeBitangents();
	
	void finish();
	void updateDrawArray(
		bool computeNormals,
		bool computeTangents,
		bool computeBitangents);

private:
	//std::vector<FaceIndex> findFaces(int vertexIndice);

private:
	//must all be same length
	std::vector<Vertex> m_vertices;
	std::vector<Uv> m_uvs;
	std::vector<Normal> m_normals;
	std::vector<Tangent> m_tangents;
	std::vector<Bitangent> m_bitangents;

	std::vector<double> m_verticesIndices;
	std::vector<double> m_uvsIndices;
	std::vector<double> m_normalsIndices;

	std::vector<int> m_vertexPerFaces;

	std::vector<std::vector<int>> m_vertexFaces;

	std::vector<VertexData> m_drawArray;

	//std::vector<std::vector<FaceIndex>> m_facesIndex; //let us find faces that include a vertex indice

	std::vector<GLuint> m_textures;
	std::vector<std::string> m_texturePaths;

	std::vector<Face> m_faces;
	std::vector<Face> m_facesUvs;
};

#endif //__MODEL_CONTAINER_H__