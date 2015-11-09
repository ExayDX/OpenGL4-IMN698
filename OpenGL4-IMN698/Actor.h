#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <gl/glew.h>
#include "Types.h"
#include "glm/glm/glm.hpp"
#include "Material.h"
#include "Types.h"


class Actor
{
public :
	virtual Matrix4x4 getModelMatrix() const; 
	virtual Matrix4x4 getModelMatrixStart() const; 
	virtual Vec3 getPosition() const;
	virtual const Material* getMaterial() const; 

	virtual void setRotation(const Matrix4x4& m);
	virtual void rotate(const Matrix4x4& m);
	virtual void translate(const Vec3& translation);

	virtual bool isVisible() const;
	virtual void setVisible(bool val);

	virtual void updateTransformationData();

protected : 
	Actor(Vec3 centerPosition, Material* material);

	Matrix4x4 m_modelMatrix;
	Material* m_material;

	bool m_isVisible; 

	// Transformations used to compute model matrix
	Matrix4x4 m_rotation;
	Matrix4x4 m_rotationStart;
	Vec3 m_position;
};

#endif // _ACTOR_H