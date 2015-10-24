#ifndef _ACTOR_H_
#define _ACTOR_H_

#include <gl/glew.h>
#include "Types.h"
#include "glm/glm/glm.hpp"
#include "Material.h"


class Actor
{
public :
	virtual Matrix4x4 getModelMatrix() const;
	virtual Matrix4x4 getModelMatrixStart() const;
	virtual const Material* getMaterial() const;
	virtual Vec3 getPosition() const;

	void applyTransformation(const glm::mat4x4& matrix);

	void setRotation(const Matrix4x4& m);
	void rotate(const Matrix4x4& m);
	void translate(const Vec3& translation);
	void updateTransformationData();

	bool isVisible() const;
	void setVisible(bool val);

protected : 
	Actor(Vec3 centerPosition, Material* material);

	Material* m_material;
	bool m_isVisible;

	//Transformations used to compute model matrix
	Matrix4x4 m_rotation;
	Matrix4x4 m_rotationStart;
	Vec3 m_position;


};

#endif // _ACTOR_H