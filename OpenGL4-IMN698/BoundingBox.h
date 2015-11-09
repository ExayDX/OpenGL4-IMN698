

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
	typedef glm::vec3 Point;

public:

	BoundingBox();
	BoundingBox(const Point& a_p);
	BoundingBox(const Point& a_p1, const Point& a_p2);

	bool overlaps(const BoundingBox& a_bb) const;
	bool contains(const Point& a_p) const;

	bool intersect(const Ray& a_r, double& a_t0, double& a_t1) const;

	friend BoundingBox combine(const BoundingBox& a_bb, const Point& a_p);
	friend BoundingBox combine(const BoundingBox& a_bb1, const BoundingBox& a_bb2);

	Point m_pMin;
	Point m_pMax;
};

#endif //__BoundingBox_H__
