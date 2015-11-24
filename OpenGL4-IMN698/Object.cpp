
#include "Object.h"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "ShaderProgram.h"
#include "BoundingBox.h"

Object::Object(glm::vec3 position, Material* material, GLuint shaderProgram)
	: Actor(position, material)
	, m_shaderProgram(shaderProgram)
	, m_numIndices(0)
	, m_animation(0)
	, m_postProcesses(LightingEffects::eNone)
{
	m_bbox = BoundingBox(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
}

Object::~Object()
{
	glDeleteVertexArrays(1, &m_VAO); 
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Object::draw()
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

	computeBoundingBox();
}

void Object::computeBoundingBox()
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

bool Object::intersect(Ray r, double& t0, double& t1)
{
	auto inverseModel = glm::inverse(Actor::getModelMatrix());
	glm::vec3 dir(
		inverseModel[0][0] * r.m_d.x + inverseModel[1][0] * r.m_d.y + inverseModel[2][0] * r.m_d.z + inverseModel[3][0] * 0,
		inverseModel[0][1] * r.m_d.x + inverseModel[1][1] * r.m_d.y + inverseModel[2][1] * r.m_d.z + inverseModel[3][1] * 0,
		inverseModel[0][2] * r.m_d.x + inverseModel[1][2] * r.m_d.y + inverseModel[2][2] * r.m_d.z + inverseModel[3][2] * 0
		);

	glm::vec3 origin(
		inverseModel[0][0] * r.m_o.x + inverseModel[1][0] * r.m_o.y + inverseModel[2][0] * r.m_o.z + inverseModel[3][0] * 1,
		inverseModel[0][1] * r.m_o.x + inverseModel[1][1] * r.m_o.y + inverseModel[2][1] * r.m_o.z + inverseModel[3][1] * 1,
		inverseModel[0][2] * r.m_o.x + inverseModel[1][2] * r.m_o.y + inverseModel[2][2] * r.m_o.z + inverseModel[3][2] * 1
		);
	Ray ray(origin, dir, std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::infinity());

	return m_bbox.intersect(ray, t0, t1);
}

Matrix4x4 Object::getModelMatrix(int frame) const
{
	Matrix4x4 model = Actor::getModelMatrix();
	Frame f;
	if (m_animation)
	{
		f = m_animation->getFrame(frame);
	}
		
	return model * f.m_transformation;
}

void Object::setAnimation(Animation* animation)
{
	m_animation = animation;
}
