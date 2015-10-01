#include "Object.h"
#include "GL/glew.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"

Object::Object(glm::vec3 position, glm::vec3 color, ShaderProgram* shaderProgram)
: m_centerPosition(position)
, m_color(color)
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
	glUseProgram(m_shaderProgram->getProgramId());
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

glm::mat4 Object::getModelMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, m_centerPosition);

	return model; 
}

GLuint Object::getShaderProgramId()
{
	return m_shaderProgram->getProgramId();
}

glm::vec3 Object::getColor()
{
	return m_color;
}
