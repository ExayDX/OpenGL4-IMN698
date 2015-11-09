#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Object.h"
#include "Actor.h"
#include "ShaderProgram.h"
#include "Material.h"

#include <gl/glew.h>
#include "glm/glm/glm.hpp"

// Forward declaration
class Object; 

/*
* m_Material corresponds to the force of the light source for each light's component,
* meaning the force of the specular, diffuse and ambient lighting a light provides.
* This is a very weird concept since it has no physical meaning (a light just lit, it
* doesn't have a force for each type of lighting). But since the concept is well 
* spread in the graphics community we decided to do like everybody else...
* Shininess doesn't mean anything for now. 
*/
class Light : public Object
{
public:
	struct AttenuationProperties
	{
		float m_constant;
		float m_linear;
		float m_quadratic;
	};

	Light(glm::vec3 aPosition, Material* material, AttenuationProperties attenuationProp, GLuint shaderProgram); 
	~Light(){};

	const AttenuationProperties& getAttenuationProperties() const { return m_attenuationProperties; };

	//Physical representation of light is a sphere for now
	virtual void defineVBO();
	virtual void defineVAO();
	virtual void defineEBO();

private : 
	AttenuationProperties m_attenuationProperties;

	double m_radius;
	GLuint m_nLats;
	GLuint m_nLongs;
};


#endif //_LIGHT_H_