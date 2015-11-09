

#ifndef __BoundingBox_H__
#define __BoundingBox_H__

#include <gl/glew.h>
#include <iostream>
#include "GLM/glm/glm.hpp"


struct Ray
{
	Ray(glm::vec3 origin, glm::vec3 direction, double min, double max) :
		m_o(origin),
		m_d(direction),
		m_mint(min),
		m_maxt(max)
	{}
	glm::vec3 m_o;
	glm::vec3 m_d;
	double m_mint;
	double m_maxt;
};

class BoundingBox 
{
public:

	BoundingBox();
	BoundingBox(const glm::vec3& a_p);
	BoundingBox(const glm::vec3& a_p1, const glm::vec3& a_p2);

	bool overlaps(const BoundingBox& a_bb) const;
	bool contains(const glm::vec3& a_p) const;

	bool intersect(const Ray& a_r, double& a_t0, double& a_t1) const;

	friend BoundingBox combine(const BoundingBox& a_bb, const glm::vec3& a_p);
	friend BoundingBox combine(const BoundingBox& a_bb1, const BoundingBox& a_bb2);

	glm::vec3 m_pMin;
	glm::vec3 m_pMax;
};

#endif //__BoundingBox_H__
