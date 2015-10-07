#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "glm/glm/glm.hpp"
#include "Actor.h"

// Forward declaration
//class Object; 

/*
* m_Material corresponds to the force of the light source for each light's component,
* meaning the force of the specular, diffuse and ambient lighting a light provides.
* This is a very weird concept since it has no physical meaning (a light just lit, it
* doesn't have a force for each type of lighting). But since the concept is well 
* spread in the graphics community we decided to do like everybody else...
* Shininess doesn't mean anything for now. 
*/
class Light : public Actor
{
public:
	// TODO: Manage syncing between physical representation and own actor (should be the same actor but...)
	//Light(Object* anObject);

	Light(glm::vec3 aPosition, Material* material); 
	~Light(){};

private : 
	//Object* m_physicalRepresentation; 
};


#endif //_LIGHT_H_