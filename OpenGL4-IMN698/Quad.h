#ifndef _QUAD_H_
#define _QUAD_H_

#include "Object.h"

class Quad : public Object
{
public : 
	Quad(glm::vec3 position, Material* material, GLuint shaderProgram); 
	virtual void draw() override; 

protected:
	virtual void defineVBO() override; 
	virtual void defineVAO() override;
	virtual void defineEBO() override; 
};

#endif 