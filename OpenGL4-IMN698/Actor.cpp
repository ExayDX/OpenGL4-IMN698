
#include "Actor.h"
#include "glm/glm/gtc/matrix_transform.hpp"

Actor::Actor(glm::vec3 centerPosition, Material* material)
	: m_material(material)
	, m_position(centerPosition)
	, m_rotation()
	, m_rotationStart()
	, m_isVisible(true)
{ }

void Actor::updateTransformationData()
{
	m_rotationStart = m_rotation;
}

Matrix4x4 Actor::getModelMatrix() const
{
	Matrix4x4 m_trans;
	m_trans = glm::translate(m_trans, m_position);
	return m_trans * m_rotation;
}

//return model matrix at the start of an operation
Matrix4x4  Actor::getModelMatrixStart() const
{
	Matrix4x4 m_trans;
	m_trans = glm::translate(m_trans, m_position);
	return m_trans * m_rotationStart;
}

const Material* Actor::getMaterial() const
{
	return m_material;
}

Vec3 Actor::getPosition() const
{
	return m_position;
}

void Actor::setRotation(const Matrix4x4& m)
{
	m_rotation = m;
}

void Actor::rotate(const Matrix4x4& m)
{
	m_rotation = m_rotationStart * m;
}

void Actor::translate(const Vec3& translation)
{
	m_position += translation;
}

bool Actor::isVisible() const
{
	return m_isVisible;
}

void Actor::setVisible(bool val)
{
	m_isVisible = val;
}