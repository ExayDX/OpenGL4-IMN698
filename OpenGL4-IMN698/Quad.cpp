#include "Quad.h"

Quad::Quad(glm::vec3 position, Material* material, GLuint shaderProgram)
: Object(position, material, shaderProgram)
{
	setupObject(); 
}

void Quad::defineVBO()
{
	m_vertices.push_back(Vertice(-1.0f, 1.0f, 0.0f));
	m_vertices.push_back(Vertice(-1.0f, -1.0f, 0.0f));
	m_vertices.push_back(Vertice(1.0f, 1.0f, 0.0f));
	m_vertices.push_back(Vertice(1.0f, -1.0f, 0.0f));

	GLfloat vertices[] = {
		// Vertex position									//Texture Coordinates 
		m_vertices[0].x, m_vertices[0].y, m_vertices[0].z,  0.0f, 1.0f,
		m_vertices[1].x, m_vertices[1].y, m_vertices[1].z, 0.0f, 0.0f,
		m_vertices[2].x, m_vertices[2].y, m_vertices[2].z, 1.0f, 1.0f,
		m_vertices[3].x, m_vertices[3].y, m_vertices[3].z, 1.0f, 0.0f,
	};

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	m_numIndices = 4; 
}
void Quad::defineVAO()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);
}
void Quad::defineEBO()
{
	
}

void Quad::draw()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_numIndices); 
	glBindVertexArray(0); 

}