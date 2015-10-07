#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "glm/glm/glm.hpp"

struct Material
{
	Material() 
		: m_ambientCoefs(1.0f, 0.0f, 0.0f)
		, m_diffuseCoefs(1.0f, 0.0f, 0.0f)
		, m_specularCoefs(1.0f, 0.0f, 0.0f)
		, m_shininess(1.0f)
	{

	}

	Material(glm::vec3 color, float shininess)
		: m_ambientCoefs(color)
		, m_diffuseCoefs(color)
		, m_specularCoefs(color)
		, m_shininess(shininess)
	{

	}

	Material(glm::vec3 ambientCoefs, glm::vec3 diffuseCoefs, glm::vec3 specularCoefs, float shininess)
		: m_ambientCoefs(ambientCoefs)
		, m_diffuseCoefs(diffuseCoefs)
		, m_specularCoefs(specularCoefs)
		, m_shininess(shininess)
	{

	}

	glm::vec3 m_ambientCoefs;
	glm::vec3 m_diffuseCoefs;
	glm::vec3 m_specularCoefs; 
	float m_shininess; 
};

#endif //_MATERIAL_H_