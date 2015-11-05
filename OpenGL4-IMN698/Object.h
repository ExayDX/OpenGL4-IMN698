#ifndef  OBJECT_H
#define	 OBJECT_H 
#include "GL/glew.h"
#include "Actor.h"
#include <GLM/glm/glm.hpp>

#include <vector>

// Forward declaration
class ShaderProgram;

//Base class for a drawable object
class Object : public Actor
{
public:
	// Ctors/Dtors
	virtual ~Object(); 

	virtual void draw() const;

	// Getters 
	virtual GLuint getShaderProgramId() { return m_shaderProgram; }

	// Setters
	void assignMaterial(Material* material) { m_material = material; } 
	void addPostProcess(GLuint aShaderProgram) { m_postProcesses.push_back(aShaderProgram); }

protected : 
	// Constructor for implementing classes
	Object(glm::vec3 position, Material* material, GLuint shaderProgram); 
	
	// Functions for Object's shape definition
	virtual void setupObject();
	virtual void defineVBO() = 0;
	virtual void defineVAO() = 0;
	virtual void defineEBO() = 0;

	// Object's look characteristics
	GLuint m_shaderProgram; 
	std::vector<GLuint> m_postProcesses; // This is used for postProcess effects that only applies to certain objects. 

	// Object's shape definition variables
	GLuint m_VBO;
	GLuint m_VAO;
	GLuint m_EBO; 

	GLuint m_numIndices;
};

#endif // ! OBJECT_H