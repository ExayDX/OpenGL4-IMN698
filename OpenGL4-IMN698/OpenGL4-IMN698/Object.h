#ifndef  OBJECT_H
#define	 OBJECT_H 
#include "GL/glew.h"
#include <GLM/glm/glm.hpp>

// Forward declaration
class ShaderProgram;

//Base class for a drawable object
class Object
{

public:

	// Ctors/Dtors
	virtual ~Object(); 

	virtual void draw() const;

	virtual glm::mat4 getModelMatrix(); 
	virtual void setupObject(); 

	// Getters 
	virtual GLuint getShaderProgramId();
	virtual glm::vec3 getColor();

protected : 
	// Constructor for implementing classes
	Object(glm::vec3 position, glm::vec3 color, ShaderProgram* shaderProgram); 
	
	// Functions for Object's shape definition
	virtual void defineVBO() = 0;
	virtual void defineVAO() = 0;
	virtual void defineEBO() = 0;

	// Object's look characteristics
	ShaderProgram* m_shaderProgram; 
	glm::vec3 m_color;

	// Object's shape definition variables
	GLuint m_EBO; 
	GLuint m_VBO; 
	GLuint m_VAO;

	glm::vec3 m_centerPosition;
	GLuint m_numIndices;
};

#endif // ! OBJECT_H