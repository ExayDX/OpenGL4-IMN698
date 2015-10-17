#include "Light.h"
#include "Object.h"

//Light::Light(Object* aRepresentation)
//	: Actor(aRepresentation->getPosition(), aRepresentation->getColor())
//	, m_physicalRepresentation(aRepresentation)
//{
//
//}

Light::Light(glm::vec3 aPosition, Material* aMaterial, AttenuationProperties attenuationProp)
	: Actor(aPosition, aMaterial)
	, m_attenuationProperties(attenuationProp)
	//, m_physicalRepresentation(nullptr)
{

}