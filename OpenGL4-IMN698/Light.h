#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <gl/glew.h>

#include "Object.h"
#include "glm/glm/glm.hpp"
//#include "Actor.h"
#include "Object.h"  // HACK : This is a bad hack, making the light completely weird, see explanations bellow. 

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
class Light : public Object // HACK : A light should only herit from actor. It doesn't have to have a physical representation and this makes it bad.
{
public:
	struct AttenuationProperties
	{
		float m_constant;
		float m_linear;
		float m_quadratic;
	};

	// TODO: Manage syncing between physical representation and own actor (should be the same actor but...)
	//Light(Object* anObject);
	Light(glm::vec3 aPosition, Material* material, AttenuationProperties attenuationProp, GLuint aShaderProgram); // HACK : Still super bad structure... blahblahblah. 

	~Light(){};

	const AttenuationProperties& getAttenuationProperties() const { return m_attenuationProperties; };

	// HACK : 
	virtual void defineVBO();
	virtual void defineVAO();
	virtual void defineEBO();

private : 
	AttenuationProperties m_attenuationProperties;

	// HACK : this is forcing the light to have a sphere physical representation. It's a hack of hack. It could have a least herit of sphere!! THis is extremely bad structure. 
	double m_radius;
	GLuint m_nLats;
	GLuint m_nLongs;
};


#endif //_LIGHT_H_