#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "Object.h"
#include "ShaderProgram.h"

#include <GLM/glm/vec3.hpp>

//base class for a drawable sphere
//using a shader program
class Sphere :public Object
{
public:
	Sphere(glm::vec3 position, glm::vec3 color, double radius, GLuint nLats, GLuint nLongs, ShaderProgram* shaderProgram);
	~Sphere(){};


	void draw() const override;
	void setShaderProgram(ShaderProgram* sp);

	void defineVBO();
	void defineEBO();
	void defineVAO(); 

private:
	double m_radius;
	GLuint m_nLats;
	GLuint m_nLongs;

};

#endif 