#include "ObjMesh.h"
#include "BoundingBox.h"

#include "glm/glm/gtc/matrix_transform.hpp"

#include "SOIL/SOIL.h"
#include <string>

ObjMesh::ObjMesh(Material* material,
	GLuint shaderProgram,
	std::vector<vertexStruct> vertices,
	std::vector<double> verticesIndices,
	std::vector<std::string> texturePaths)
	:
	Object(glm::vec3(0, 0, 0), material, shaderProgram),
	m_vertices(vertices),
	m_verticesIndices(verticesIndices)
{
	setupObject();

	for (int i = 0; i < texturePaths.size(); ++i)
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
		unsigned char* image = SOIL_load_image(texturePaths[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

		m_textures.push_back(texture);
	}
}

void ObjMesh::defineVBO()
{
	std::vector<GLfloat> vertices;

	for (int i = 0; i < m_vertices.size(); i++)
	{
		vertices.push_back(m_vertices[i].vertex.x);
		vertices.push_back(m_vertices[i].vertex.y);
		vertices.push_back(m_vertices[i].vertex.z);

		vertices.push_back(m_vertices[i].normal.x);
		vertices.push_back(m_vertices[i].normal.y);
		vertices.push_back(m_vertices[i].normal.z);

		vertices.push_back(m_vertices[i].uv.x);
		vertices.push_back(m_vertices[i].uv.y);

		vertices.push_back(m_vertices[i].normal.x);
		vertices.push_back(m_vertices[i].normal.y);
		vertices.push_back(m_vertices[i].normal.z);

		vertices.push_back(m_vertices[i].tangent.x);
		vertices.push_back(m_vertices[i].tangent.y);
		vertices.push_back(m_vertices[i].tangent.z);

		vertices.push_back(m_vertices[i].bitangent.x);
		vertices.push_back(m_vertices[i].bitangent.y);
		vertices.push_back(m_vertices[i].bitangent.z);
	}

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
}

void ObjMesh::defineEBO()
{
	std::vector<GLuint> indices;
	for (int i = 0; i < m_verticesIndices.size(); ++i)
	{
		//indices in obj start at 1
		indices.push_back(m_verticesIndices[i] - 1);
	}

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	m_numIndices = indices.size();

}

void ObjMesh::defineVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);


	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(11 * sizeof(GLfloat)));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 17 * sizeof(GLfloat), (GLvoid*)(14 * sizeof(GLfloat)));
	glEnableVertexAttribArray(5);

	glBindVertexArray(0);
}


void ObjMesh::draw() const
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
		glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
		glBindVertexArray(0);
	}
}

void ObjMesh::computeBoundingBox()
{
	BoundingBox bbox;

	double	minX = std::numeric_limits<double>::max(),
		minY = std::numeric_limits<double>::max(),
		minZ = std::numeric_limits<double>::max();
	double	maxX = std::numeric_limits<double>::lowest(),
		maxY = std::numeric_limits<double>::lowest(),
		maxZ = std::numeric_limits<double>::lowest();

	//go through all vertices and compute bounding box
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		if (m_vertices[i].vertex.x < minX)
			minX = m_vertices[i].vertex.x;
		if (m_vertices[i].vertex.y < minY)
			minY = m_vertices[i].vertex.y;
		if (m_vertices[i].vertex.z < minZ)
			minZ = m_vertices[i].vertex.z;

		if (m_vertices[i].vertex.x > maxX)
			maxX = m_vertices[i].vertex.x;
		if (m_vertices[i].vertex.y > maxY)
			maxY = m_vertices[i].vertex.y;
		if (m_vertices[i].vertex.z > maxZ)
			maxZ = m_vertices[i].vertex.z;
	}

	m_bbox = BoundingBox(BoundingBox::Point(minX, minY, minZ), BoundingBox::Point(maxX, maxY, maxZ));
}