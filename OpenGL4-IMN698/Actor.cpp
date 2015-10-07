#include "Actor.h"
#include "glm/glm/gtc/matrix_transform.hpp"

Actor::Actor(glm::vec3 centerPosition, Material* material)
	: m_material(material)
	, m_centerPosition(centerPosition)
{
	m_modelMatrix = glm::translate(m_modelMatrix, m_centerPosition);
}