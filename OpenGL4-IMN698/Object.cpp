#include "Object.h"
#include "GL/glew.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"

Object::Object(glm::vec3 position, Material* material, GLuint shaderProgram)
	: Actor(position, material)
	, m_shaderProgram(shaderProgram)
	, m_numIndices(0)
{
	
}

Object::~Object()
{
	glDeleteVertexArrays(1, &m_VAO); 
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Object::draw() const
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO); 
	glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0); 
	glBindVertexArray(0);
}

void Object::setupObject()
{
	defineVBO();
	defineEBO();
	defineVAO(); 
}