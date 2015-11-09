//
//  BoundingBox.cpp
//  RayTracer
//

#include "BoundingBox.h"

#include <limits>
#include <algorithm>


BoundingBox::BoundingBox()
{
	m_pMin = Point(std::numeric_limits<double>::max());
	m_pMax = Point(std::numeric_limits<double>::lowest());
}

BoundingBox::BoundingBox(const Point& a_p)
{
	m_pMin = a_p;
	m_pMax = a_p;
}

BoundingBox::BoundingBox(const Point& a_p1, const Point& a_p2)
{
	//find min/max values of x/y/z
	double minX = std::min(a_p1.x, a_p2.x);
	double minY = std::min(a_p1.y, a_p2.y);
	double minZ = std::min(a_p1.z, a_p2.z);

	double maxX = std::max(a_p1.x, a_p2.x);
	double maxY = std::max(a_p1.y, a_p2.y);
	double maxZ = std::max(a_p1.z, a_p2.z);

	m_pMin = Point(minX, minY, minZ);
	m_pMax = Point(maxX, maxY, maxZ);
}

bool BoundingBox::overlaps(const BoundingBox& a_bb) const
{
	return a_bb.contains(m_pMin) || a_bb.contains(m_pMax);
}

bool BoundingBox::contains(const Point& a_p) const
{
	return a_p.x <= m_pMax.x && a_p.x >= m_pMin.x &&
		a_p.y <= m_pMax.y && a_p.y >= m_pMin.y &&
		a_p.z <= m_pMax.z && a_p.z >= m_pMin.z;
}

bool BoundingBox::intersect(const Ray& a_r, double& a_t0, double& a_t1) const
{
	//division by 0 is handled by double
	//result is +INF or -INF
	//It is handled properly by std::min/max
	double t1 = (m_pMin.x - a_r.m_o.x) / a_r.m_d.x;
	double t2 = (m_pMin.y - a_r.m_o.y) / a_r.m_d.y;
	double t3 = (m_pMin.z - a_r.m_o.z) / a_r.m_d.z;

	double t4 = (m_pMax.x - a_r.m_o.x) / a_r.m_d.x;
	double t5 = (m_pMax.y - a_r.m_o.y) / a_r.m_d.y;
	double t6 = (m_pMax.z - a_r.m_o.z) / a_r.m_d.z;

	a_t0 = std::max(std::max(std::min(t1, t4), std::min(t2, t5)), std::min(t3, t6));
	a_t1 = std::min(std::min(std::max(t1, t4), std::max(t2, t5)), std::max(t3, t6));

	if (contains(a_r.m_o))
		a_t0 = a_r.m_mint;

	//ensure first collision is made within the lifespan of the ray
	bool a_t0InBound = a_t0 >= a_r.m_mint && a_t0 <= a_r.m_maxt && std::isfinite(a_t0);

	return a_t0InBound && a_t0 <= a_t1;
}

BoundingBox combine(const BoundingBox& a_bb, const Point& a_p)
{
	double minX = std::min(a_bb.m_pMin.x, a_p.x);
	double minY = std::min(a_bb.m_pMin.y, a_p.y);
	double minZ = std::min(a_bb.m_pMin.z, a_p.z);
	double maxX = std::max(a_bb.m_pMax.x, a_p.x);
	double maxY = std::max(a_bb.m_pMax.y, a_p.y);
	double maxZ = std::max(a_bb.m_pMax.z, a_p.z);

	return BoundingBox(Point(minX, minY, minZ), Point(maxX, maxY, maxZ));
}

BoundingBox combine(const BoundingBox& a_bb1, const BoundingBox& a_bb2)
{
	double minX = std::min(a_bb1.m_pMin.x, a_bb2.m_pMin.x);
	double minY = std::min(a_bb1.m_pMin.y, a_bb2.m_pMin.y);
	double minZ = std::min(a_bb1.m_pMin.z, a_bb2.m_pMin.z);
	double maxX = std::max(a_bb1.m_pMax.x, a_bb2.m_pMax.x);
	double maxY = std::max(a_bb1.m_pMax.y, a_bb2.m_pMax.y);
	double maxZ = std::max(a_bb1.m_pMax.z, a_bb2.m_pMax.z);

	return BoundingBox(Point(minX, minY, minZ), Point(maxX, maxY, maxZ));
}