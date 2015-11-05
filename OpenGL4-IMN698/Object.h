#ifndef  OBJECT_H
#define	 OBJECT_H 

#include "ShaderProgram.h"
#include "BoundingBox.h"
#include "Actor.h"

#include <gl/glew.h>
#include <GLM/glm/glm.hpp>

#include <vector>

// Forward declaration
class ShaderProgram;


//Base class for a drawable object
class Object : public Actor
{
public:
	typedef glm::vec3 Vertice;

public:
	// Ctors/Dtors
	virtual ~Object(); 

	virtual void draw() const;

	// Getters 
	GLuint getShaderProgramId() { return m_shaderProgram; }

	// Setters
	void assignMaterial(Material* material) { m_material = material; } 
	void addPostProcess(GLuint aShaderProgram) { m_postProcesses.push_back(aShaderProgram); }

	void changeShader(ShaderProgram* sp);

	virtual void computeBoundingBox();
	bool intersect(Ray r, double& t0, double& t1);

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
	std::vector<GLuint> m_postProcesses; // These will be applyed in the order they have been set in. 

	// Object's shape definition variables
	GLuint m_VBO;
	GLuint m_VAO;
	GLuint m_EBO; 

	GLuint m_numIndices;


	std::vector<Vertice> m_vertices;
	BoundingBox m_bbox;
};

#endif // ! OBJECT_H