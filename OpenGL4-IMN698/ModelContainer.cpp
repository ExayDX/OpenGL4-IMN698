

#include "ModelContainer.h"

#include "SOIL/SOIL.h"

#include <string>
#include <map>
#include <set>

ModelContainer::ModelContainer(Material* material, GLuint shaderProgram, std::vector<Vec3> backgroundColor) :
	Object(glm::vec3(0, 0, 0), material, shaderProgram)
	, m_backgroundColor(backgroundColor)
	, m_isolatedPatchCoeff()
	, m_backgroundDistance(DEFAULT_BACKGROUND_DISTANCE)
{
}

void ModelContainer::computeBoundingBox()
{
	BoundingBox bbox;


	double	minX = std::numeric_limits<double>::max(),
			minY = std::numeric_limits<double>::max(),
			minZ = std::numeric_limits<double>::max();
	double	maxX = std::numeric_limits<double>::lowest(),
			maxY = std::numeric_limits<double>::lowest(),
			maxZ = std::numeric_limits<double>::lowest();

	assert(m_vertices.size() > 0);

	//go through all vertices and compute bounding box
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		if (m_vertices[i].x < minX)
			minX = m_vertices[i].x;
		if (m_vertices[i].y < minY)
			minY = m_vertices[i].y;
		if (m_vertices[i].z < minZ)
			minZ = m_vertices[i].z;

		if (m_vertices[i].x > maxX)
			maxX = m_vertices[i].x;
		if (m_vertices[i].y > maxY)
			maxY = m_vertices[i].y;
		if (m_vertices[i].z > maxZ)
			maxZ = m_vertices[i].z;
	}

	m_bbox = BoundingBox(Point(minX, minY, minZ), Point(maxX, maxY, maxZ));
}

void ModelContainer::addVertex(Vertex vertex)
{
	m_vertices.push_back(vertex);
	m_vertexFaces.push_back(std::vector<int>());
}

void ModelContainer::addNormal(Normal normal)
{
	m_normals.push_back(normal);
}

void ModelContainer::addUv(Uv uv)
{
	m_uvs.push_back(uv);
}

void ModelContainer::addVertexIndex(int index)
{
	m_verticesIndices.push_back(index);
}
void ModelContainer::removeVertexIndex()
{
	m_verticesIndices.pop_back();
}

void ModelContainer::addNormalIndex(int index)
{
	m_normalsIndices.push_back(index);
}

void ModelContainer::removeNormalIndex()
{
	m_normalsIndices.pop_back();
}

void ModelContainer::addUvIndex(int index)
{
	m_uvsIndices.push_back(index);
}

void ModelContainer::removeUvIndex()
{
	m_uvsIndices.pop_back();
}

void ModelContainer::addVertexPerFace(int num)
{
	m_vertexPerFaces.push_back(num);
}

void ModelContainer::addTexturePath(std::string path)
{
	//first texture is always the diffuse texture, 
	//we need to load it in order to remove background faces if needed
	if (m_texturePaths.empty())
	{
		m_image = SOIL_load_image(path.c_str(), &m_width, &m_height, 0, SOIL_LOAD_RGB);
	}
	m_texturePaths.push_back(path);
}

void ModelContainer::smoothNormals()
{
	m_normalsIndices.clear();
	m_normals.clear();
	m_tangents.clear();
	m_bitangents.clear();

	int currentIndex = 1;
	int currentVert = 0;

	//m_faces contains vertex indices for its face
	//m_vertexFaces contains faces for a given vertex index
	for (int f = 0; f < m_faces.size(); ++f)
	{
		//for each vertex in a face
		for (int i = 0; i < m_faces[f].size(); ++i)
		{
			int vertexIndex = m_faces[f][i];

			std::vector<int> adjacentFacesIndexes = m_vertexFaces[vertexIndex];

			std::vector<Normal> adjacentFacesNormals;
			std::vector<Tangent> adjacentFacesTangents;
			std::vector<Bitangent> adjacentFacesBiTangents;

			for each (int adjacentFaceIndex in adjacentFacesIndexes)
			{
				//get vertices of each face
				std::vector<int> verticesIndexes = m_faces[adjacentFaceIndex];
				std::vector<int> uvsIndexes = m_facesUvs[adjacentFaceIndex];

				//calculate surface normal
				Normal surfaceNormal = glm::cross(
					m_vertices[verticesIndexes[0]] - m_vertices[verticesIndexes[1]],
					m_vertices[verticesIndexes[0]] - m_vertices[verticesIndexes[2]]);

				surfaceNormal = glm::normalize(surfaceNormal);
				adjacentFacesNormals.push_back(surfaceNormal);

				glm::vec3 pos1(m_vertices[verticesIndexes[0]]);
				glm::vec3 pos2(m_vertices[verticesIndexes[1]]);
				glm::vec3 pos3(m_vertices[verticesIndexes[2]]);
				glm::vec2 uv1(m_uvs[uvsIndexes[0]]);
				glm::vec2 uv2(m_uvs[uvsIndexes[1]]);
				glm::vec2 uv3(m_uvs[uvsIndexes[2]]);
				glm::vec3 edge1 = pos2 - pos1;
				glm::vec3 edge2 = pos3 - pos1;
				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;
				GLfloat d = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

				Tangent tangent;
				tangent.x = d * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
				tangent.y = d * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
				tangent.z = d * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
				tangent = glm::normalize(tangent);
				adjacentFacesTangents.push_back(tangent);

				Bitangent bitangent;
				bitangent.x = d * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
				bitangent.y = d * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
				bitangent.z = d * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
				bitangent = glm::normalize(bitangent);
				adjacentFacesBiTangents.push_back(bitangent);
			}

			//normal at current vertex must be a rounding of all face's normal that are: < angle
			Normal avg;
			for each (Normal n in adjacentFacesNormals)
			{
				avg.x += n.x;
				avg.y += n.y;
				avg.z += n.z;
			}

			avg = glm::normalize(avg);
			m_normals.push_back(avg);
			m_normalsIndices.push_back(currentIndex + i);

			Tangent avgTangent;
			for each (Tangent t in adjacentFacesTangents)
			{
				avgTangent.x += t.x;
				avgTangent.y += t.y;
				avgTangent.z += t.z;
			}

			avgTangent = glm::normalize(avgTangent);
			m_tangents.push_back(avgTangent);

			Bitangent avgBitangent;
			for each (Bitangent t in adjacentFacesBiTangents)
			{
				avgBitangent.x += t.x;
				avgBitangent.y += t.y;
				avgBitangent.z += t.z;
			}

			avgBitangent = glm::normalize(avgBitangent);
			m_bitangents.push_back(avgBitangent);
		}

		//currentVert += m_faces[f].size();
		currentIndex += m_faces[f].size();
		currentVert += m_faces[f].size();
	}

	updateDrawArray(false, false, false);
	setupObject();
}

void ModelContainer::defineVBO()
{
	std::vector<GLfloat> vertices;

	for (int i = 0; i < m_drawArray.size(); i++)
	{
		vertices.push_back(m_drawArray[i].m_vertex.x);
		vertices.push_back(m_drawArray[i].m_vertex.y);
		vertices.push_back(m_drawArray[i].m_vertex.z);

		vertices.push_back(m_drawArray[i].m_normal.x);
		vertices.push_back(m_drawArray[i].m_normal.y);
		vertices.push_back(m_drawArray[i].m_normal.z);

		vertices.push_back(m_drawArray[i].m_uv.x);
		vertices.push_back(m_drawArray[i].m_uv.y);

		vertices.push_back(m_drawArray[i].m_tangent.x);
		vertices.push_back(m_drawArray[i].m_tangent.y);
		vertices.push_back(m_drawArray[i].m_tangent.z);

		vertices.push_back(m_drawArray[i].m_bitangent.x);
		vertices.push_back(m_drawArray[i].m_bitangent.y);
		vertices.push_back(m_drawArray[i].m_bitangent.z);
	}

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
}

void ModelContainer::defineEBO()
{
}

void ModelContainer::defineVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);

	glBindVertexArray(0);
}


void ModelContainer::draw()
{
	if (isVisible())
	{
		//uniforms for texture are: texture0, texture1, texture2, ...
		for (int i = 0; i < m_textures.size(); ++i)
		{
			std::string uniformName = "texture" + std::to_string(i);
			glActiveTexture(GL_TEXTURE0 + i * 0x0001);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]);
			glUniform1i(glGetUniformLocation(m_shaderProgram, uniformName.c_str()), i);

		}

		glBindVertexArray(m_VAO);
		glDrawArrays(GL_TRIANGLES, 0, m_drawArray.size());
		glBindVertexArray(0);
	}
}

void ModelContainer::updateBackgroundRemovalDistance(unsigned int newBackgroundDistance)
{
	m_backgroundDistance = newBackgroundDistance;
	finish();
}

void ModelContainer::removeIsolatedPatch(int isolatedCoefficient)
{
	m_isolatedPatchCoeff = isolatedCoefficient;
	finish();
}

void ModelContainer::removeBackground(const std::vector<Vec3>& backgroundColors)
{
	for (int i = 0; i < backgroundColors.size(); ++i)
	{
		Vec3 color = backgroundColors[i];
		m_backgroundColor.push_back(color);
	}
	
	finish();
}

void ModelContainer::computeBackgroundPlane()
{
	//check all vertices with the background colors
	int currentIndex = 0;
	Normal avgNormal;
	double dCoeff = 0;
	int nbCoeff = 0;

	for (int i = 0; i < m_vertexPerFaces.size(); i++)
	{
		int vertexPerPolygon = m_vertexPerFaces[i];

		if (vertexPerPolygon != 3)
		{
			assert(false);
		}

		for (int j = 0; j < vertexPerPolygon; j++)
		{
			if (isBackgroundFace(currentIndex, vertexPerPolygon))
			{
				//compute an average normal. This normal is going to be the background plane normal
				//We suppose that faces detected as background that are not background are negligeable
				if (!m_normals.empty())
				{
					Normal normal = m_normals[m_normalsIndices[currentIndex + j] - 1];
					avgNormal.x += normal.x;
					avgNormal.y += normal.y;
					avgNormal.z += normal.z;
					avgNormal = glm::normalize(avgNormal);

					//compute average plane equation
					Vec3 vertex = m_vertices[m_verticesIndices[currentIndex + j] - 1];

					dCoeff += (vertex.x * normal.x + vertex.y * normal.y + vertex.z * normal.z);
					nbCoeff++;
				}
			}
		}

		currentIndex += vertexPerPolygon;
	}

	m_backgroundPlane.a = avgNormal.x;
	m_backgroundPlane.b = avgNormal.y;
	m_backgroundPlane.c = avgNormal.z;
	m_backgroundPlane.d = dCoeff / nbCoeff;

}

bool ModelContainer::isBackgroundFace(
	int currentIndex, 
	int vertexPerPolygon)
{
	if (m_backgroundColor.empty())
		return false;

	//check if any of the vertices is a background
	for (int i = 0; i < vertexPerPolygon; ++i)
	{
		//check if vertex is on the background plane by calculating distance
		Vec3 vertex = m_vertices[m_verticesIndices[currentIndex + i] - 1];
		Normal normal = m_normals[m_normalsIndices[currentIndex + i] - 1];

		double dist = abs(vertex.x * m_backgroundPlane.a + vertex.y * m_backgroundPlane.b + vertex.z * m_backgroundPlane.c - m_backgroundPlane.d) /
			std::sqrt(m_backgroundPlane.a*m_backgroundPlane.a + m_backgroundPlane.b*m_backgroundPlane.b + m_backgroundPlane.c*m_backgroundPlane.c);

		if (dist < m_backgroundDistance)
			return true;

		//check if its part of the background color
		unsigned int uvIndex = m_uvsIndices[currentIndex + i];

		Vec3 color = getColorForUvIndex(uvIndex);
		int r = color.r;
		int g = color.g;
		int b = color.b;

		for (int j = 0; j < m_backgroundColor.size(); ++j)
		{
			int backgroundR = m_backgroundColor[j].r - 10;
			int backgroundG = m_backgroundColor[j].g - 10;
			int backgroundB = m_backgroundColor[j].b - 10;

			int backgroundR2 = m_backgroundColor[j].r + 10;
			int backgroundG2 = m_backgroundColor[j].g + 10;
			int backgroundB2 = m_backgroundColor[j].b + 10;

			//if at least one color correspond, face is part of the background
			if (r > backgroundR && r < backgroundR2
				&& g > backgroundG && g < backgroundG2
				&& b > backgroundB && b < backgroundB2)
				return true;
		}
	}
	return false;
}

bool ModelContainer::isIsolatedPatch(
	int currentIndex)
{
	std::set<int> adjacentFaceIndices;

	//check neighbors
	std::vector<int> adjacentFacesIndexesToExplore = m_vertexFaces[m_verticesIndices[currentIndex] - 1];

	std::set<int> toExplore;
	for each (int t in adjacentFacesIndexesToExplore)
	{
		toExplore.insert(t);
	}

	while (!toExplore.empty())
	{
		//vertex is not isolated if at least m_isolatedPatchCoeff close neighbors
		if (adjacentFaceIndices.size() > m_isolatedPatchCoeff)
			return false;

		//from a face index, get all its vertices
		int currentAdjacentFaceIndex = *(toExplore.begin());
		toExplore.erase(toExplore.begin());

		//insert face not explorer yet
		if (adjacentFaceIndices.find(currentAdjacentFaceIndex) == adjacentFaceIndices.end())
			adjacentFaceIndices.insert(currentAdjacentFaceIndex);

		//from all those vertices, get adjacent faces and push them to be explored
		std::vector<int> verticesIndexes = m_faces[currentAdjacentFaceIndex];
		for each (int vertexIndex in verticesIndexes)
		{
			std::vector<int> adjacentFaces = m_vertexFaces[vertexIndex];

			for each (int adjacentFaceIndex in adjacentFaces)
			{
				if (toExplore.find(adjacentFaceIndex) == toExplore.end() && 
					adjacentFaceIndices.find(adjacentFaceIndex) == adjacentFaceIndices.end())
					toExplore.insert(adjacentFaceIndex);
			}
		}

	}

	return true;
}

void ModelContainer::updateDrawArray(
	bool computeNormals, 
	bool computeTangents, 
	bool computeBitangents)
{
	int currentIndex = 0;

	m_drawArray.clear();
	m_faces.clear();

	for each (auto v in m_vertexFaces)
	{
		v.clear();
	}

	if (!m_backgroundColor.empty())
		computeBackgroundPlane();

	for (int i = 0; i < m_vertexPerFaces.size(); i++)
	{
		int vertexPerPolygon = m_vertexPerFaces[i];
		m_faces.push_back(std::vector<int>());
		m_facesUvs.push_back(std::vector<int>());

		for (int j = 0; j < vertexPerPolygon; j++)
		{
			//Data for normal interpollation
			m_vertexFaces[m_verticesIndices[currentIndex + j] - 1].push_back(i);
			m_faces[i].push_back(m_verticesIndices[currentIndex + j] - 1);
			m_facesUvs[i].push_back(m_uvsIndices[currentIndex + j] - 1);
		}
		currentIndex += vertexPerPolygon;
	}

	currentIndex = 0;

	for (int i = 0; i < m_vertexPerFaces.size(); i++)
	{
		int vertexPerPolygon = m_vertexPerFaces[i];

		if (vertexPerPolygon != 3)
		{
			assert(false);
		}

		//don't add to the model faces that are part of the background
		if (!isBackgroundFace(currentIndex, vertexPerPolygon) && !isIsolatedPatch(currentIndex))
		{

			for (int j = 0; j < vertexPerPolygon; j++)
			{

				ModelContainer::VertexData vert;
				vert.m_vertex = m_vertices[m_verticesIndices[currentIndex + j] - 1];

				if (!m_normals.empty())
				{
					vert.m_normal = glm::normalize(m_normals[m_normalsIndices[currentIndex + j] - 1]);
				}
				if (!m_uvs.empty())
				{
					vert.m_uv = m_uvs[m_uvsIndices[currentIndex + j] - 1];

				}
				if (m_tangents.size() > currentIndex + j)
				{
					vert.m_tangent = m_tangents[currentIndex + j];
				}
				if (m_bitangents.size() > currentIndex + j)
				{
					vert.m_bitangent = m_bitangents[currentIndex + j];
				}

				//Calculation for tangents and bitangents
				glm::vec3 pos1(m_vertices[m_verticesIndices[currentIndex + j] - 1]);
				glm::vec3 pos2(m_vertices[m_verticesIndices[currentIndex + (j + 1) % 3] - 1]);
				glm::vec3 pos3(m_vertices[m_verticesIndices[currentIndex + (j + 2) % 3] - 1]);
				glm::vec2 uv1(m_uvs[m_uvsIndices[currentIndex + j] - 1]);
				glm::vec2 uv2(m_uvs[m_uvsIndices[currentIndex + (j + 1) % 3] - 1]);
				glm::vec2 uv3(m_uvs[m_uvsIndices[currentIndex + (j + 2) % 3] - 1]);
				glm::vec3 edge1 = pos2 - pos1;
				glm::vec3 edge2 = pos3 - pos1;
				glm::vec2 deltaUV1 = uv2 - uv1;
				glm::vec2 deltaUV2 = uv3 - uv1;
				GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

				if (computeTangents)
				{
					Tangent tangent;
					tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
					tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
					tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
					tangent = glm::normalize(tangent);

					vert.m_tangent = tangent;
					m_tangents.push_back(tangent);
				}
				if (computeBitangents)
				{
					Bitangent bitangent;
					bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
					bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
					bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
					bitangent = glm::normalize(bitangent);

					vert.m_bitangent = bitangent;
					m_bitangents.push_back(bitangent);
				}

				m_drawArray.push_back(vert);
			}
		}

		currentIndex += vertexPerPolygon;
	}
}

void ModelContainer::finish()
{
	//Build draw array
	updateDrawArray(true, true, true);

	setupObject();

	m_textures.clear();
	for (int i = 0; i < m_texturePaths.size(); ++i)
	{
		GLuint texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object
		// Set our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Load, create texture and generate mipmaps
		int width, height;
		unsigned char* image = SOIL_load_image(m_texturePaths[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

		m_textures.push_back(texture);
	}
}

//Texture0 is always the diffuse texture so its the one we compare to
Vec3 ModelContainer::getColorForUvIndex(int uvIndex)
{
	Vec2 uv = m_uvs[uvIndex - 1];

	Vec2 pixelPos(int(uv.x * m_width), int((1 - uv.y) * m_height));

	int index = (pixelPos.x * 3) + (pixelPos.y * (m_width * 3));
	unsigned char r = m_image[index];
	unsigned char g = m_image[index+1];
	unsigned char b = m_image[index+2];

	return Vec3((int)r, (int)g, (int)b);
}
