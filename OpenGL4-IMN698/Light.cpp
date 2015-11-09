
#include "Light.h"
#include "Sphere.h"

#include <gl/glew.h>
#include "GLM/glm/gtc/constants.hpp"

Light::Light(glm::vec3 aPosition, Material* aMaterial, AttenuationProperties attenuationProp, GLuint aShaderProgram)
	: Object(aPosition, aMaterial, aShaderProgram)
	, m_attenuationProperties(attenuationProp)
	, m_radius(1)
	, m_nLats(100)
	, m_nLongs(40)
{
	setupObject(); 
}


void Light::defineEBO()
{
	std::vector<GLuint> indices;
	for (int latNumber = 0; latNumber < m_nLats; latNumber++)
	{
		for (int longNumber = 0; longNumber < m_nLongs; longNumber++)
		{
			int first = (latNumber * (m_nLongs + 1)) + longNumber;
			int second = first + m_nLongs + 1;
			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(first + 1);

			indices.push_back(second);
			indices.push_back(second + 1);
			indices.push_back(first + 1);
		}
	}

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	m_numIndices = indices.size();

}

void Light::defineVBO()
{
	std::vector<GLfloat> vertices;

	for (int latNumber = 0; latNumber <= m_nLats; latNumber++)
	{
		GLfloat theta = latNumber * glm::pi<float>() / m_nLats;
		GLfloat sinTheta = sin(theta);
		GLfloat cosTheta = cos(theta);

		for (int longNumber = 0; longNumber <= m_nLongs; longNumber++)
		{
			GLfloat phi = longNumber * 2 * glm::pi<float>() / m_nLongs;
			GLfloat sinPhi = sin(phi);
			GLfloat cosPhi = cos(phi);

			GLfloat x = cosPhi * sinTheta;
			GLfloat y = cosTheta;
			GLfloat z = sinPhi * sinTheta;
			GLfloat u = 1 - (longNumber / m_nLongs);
			GLfloat v = 1 - (latNumber / m_nLats);

			// Set the vertex's position
			vertices.push_back(m_radius * x);
			vertices.push_back(m_radius * y);
			vertices.push_back(m_radius * z);

			// Set the vertex's normal
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
	}

	for (int i = 0; i < vertices.size(); i += 3)
	{
		m_vertices.push_back(Vertice(vertices[i], vertices[i + 1], vertices[i + 2]));
	}

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
}

void Light::defineVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}