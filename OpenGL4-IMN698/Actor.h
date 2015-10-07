#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "glm/glm/glm.hpp"
#include "Material.h"

class Actor
{
public :
	virtual glm::mat4 getModelMatrix() { return m_modelMatrix; }
	virtual const Material* getMaterial() { return m_material; }
	virtual glm::vec3 getPosition() { return m_centerPosition; }

protected : 
	Actor(glm::vec3 centerPosition, Material* material);

	Material* m_material;
	glm::vec3 m_centerPosition;
	glm::mat4 m_modelMatrix;
};

#endif // _ACTOR_H