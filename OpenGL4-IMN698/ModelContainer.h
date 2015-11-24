
#ifndef __MODEL_CONTAINER_H__
#define __MODEL_CONTAINER_H__

#include "Object.h"

#include "glm/glm/glm.hpp"

#include <vector>

class Object;

// container of a 3d model
class ModelContainer: public Object
{
	const unsigned int DEFAULT_BACKGROUND_DISTANCE = 2; //TODO find an heuristic to determine this variable

public:
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

	struct Plane
	{
		double a, b, c, d;
	};


public:
	//Object member functions
	void defineVBO();
	void defineEBO();
	void defineVAO();

	virtual void draw();

public:
	ModelContainer(Material* material, GLuint shaderProgram, std::vector<Vec3> backgroundColor);

	void addVertex(Vertex vertex);
	void addNormal(Normal normal);
	void addUv(Uv uv);

	void addVertexIndex(int index);
	void removeVertexIndex();

	void addNormalIndex(int index);
	void removeNormalIndex();

	void addUvIndex(int index);
	void removeUvIndex();

	void addVertexPerFace(int num);

	void addTexturePath(std::string path);

	void triangulate();
	void getTriangles();

	void computeNormals();
	void smoothNormals();
	void removeBackground(const std::vector<Vec3>& backgroundColors);
	void removeIsolatedPatch(int isolatedCoefficient);
	void updateBackgroundRemovalDistance(unsigned int newBackgroundDistance);

	void computeBackgroundPlane();

	void finish();
	void preProcess();

	void updateDrawArray(
		bool computeNormals,
		bool computeTangents,
		bool computeBitangents);

	Vec3 getColorForUvIndex(int uvIndex);

private:
	bool isBackgroundFace(int currentIndex, int vertexPerPolygon);
	bool isIsolatedPatch(int currentIndex);
	virtual void computeBoundingBox();
private:
	int m_width;
	int m_height;
	unsigned char* m_image; //diffuse texture

	//data
	std::vector<Vertex> m_vertices;
	std::vector<Uv> m_uvs;
	std::vector<Normal> m_normals;
	std::vector<Tangent> m_tangents;
	std::vector<Bitangent> m_bitangents;

	//indices
	std::vector<double> m_verticesIndices;
	std::vector<double> m_uvsIndices;
	std::vector<double> m_normalsIndices;

	//faces
	std::vector<int> m_vertexPerFaces;
	
	//textures
	std::vector<GLuint> m_textures;
	std::vector<std::string> m_texturePaths;

	//data for adjacent faces, needed for interpolation
	std::vector<Face> m_faces;
	std::vector<Face> m_facesUvs;
	std::vector<Face> m_vertexFaces;

	std::vector<VertexData> m_drawArray;

	Plane m_backgroundPlane;
	std::vector<Vec3> m_backgroundColor;
	unsigned int m_isolatedPatchCoeff;
	unsigned int m_backgroundDistance;
};

#endif //__MODEL_CONTAINER_H__