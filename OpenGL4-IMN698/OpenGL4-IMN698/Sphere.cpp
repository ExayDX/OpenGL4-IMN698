

#define GLFW_INCLUDE_GLU

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "GLM/glm/gtc/constants.hpp"

#include "Sphere.h"

#define space 10

Sphere::Sphere(glm::vec3 position, glm::vec3 color, double radius, GLuint nLats, GLuint nLongs, ShaderProgram* shaderProgram)
	: Object(position, color, shaderProgram)
	, m_radius(radius)
	, m_nLats(nLats)
	, m_nLongs(nLongs)
{

}

void Sphere::draw() const
{
	glUseProgram(m_shaderProgram->getProgramId());
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::setShaderProgram(ShaderProgram* sp)
{
	m_shaderProgram = sp;
}

// Code from http://learningwebgl.com/blog/?p=1253 (converted to c++)
void Sphere::defineVBO()
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

			//normalData.push(x);
			//normalData.push(y);
			//normalData.push(z);
			vertices.push_back(m_radius * x);
			vertices.push_back(m_radius * y);
			vertices.push_back(m_radius * z);
		}
	}

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	
}

void Sphere::defineEBO()
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

void Sphere::defineVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

}