#ifndef  OBJECT_H
#define	 OBJECT_H 

#include "GL/glew.h"
#include <GLM/glm/glm.hpp>

#include "Actor.h"
#include "BoundingBox.h"
#include "ShaderProgram.h"
#include "Animable.h"
#include "LightingEffects.h"

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

	virtual void draw();

	Matrix4x4 getModelMatrix(int frame) const;

	// Getters 
	GLuint getShaderProgramId() { return m_shaderProgram; }
	LightingEffects getPostProcesses() { return m_postProcesses; }

	// Setters
	void assignMaterial(Material* material) { m_material = material; } 
	void setPostProcesses(LightingEffects aFlag) { m_postProcesses = aFlag; } // TODO : m_postProcesses | flag;// Takes a flag containing all postprocess to be executed
	void addPostProcess(LightingEffects aFlag){ m_postProcesses | aFlag; }
	void rmvPostProcess(LightingEffects aFlag){ m_postProcesses = aFlag & ~m_postProcesses; }
	void setAnimation(Animation* animation);
	
	void updateShader(GLuint& aNewShaderProgram) { m_shaderProgram = aNewShaderProgram; };
	void changeShader(ShaderProgram* sp);

	virtual void computeBoundingBox(); 
	virtual bool intersect(Ray r, double& t0, double& t1); 

protected : 
	typedef glm::vec3 Vertice; 

	// Constructor for implementing classes
	Object(glm::vec3 position, Material* material, GLuint shaderProgram); 
	
	// Functions for Object's shape definition
	virtual void setupObject();
	virtual void defineVBO() = 0;
	virtual void defineVAO() = 0;
	virtual void defineEBO() = 0;

	// Object's look characteristics

	GLuint m_shaderProgram; 
	LightingEffects m_postProcesses; // Flag of postProcess effects 

	// Object's shape definition variables
	GLuint m_VBO;
	GLuint m_VAO;
	GLuint m_EBO; 

	GLuint m_numIndices;

	std::vector<Vertice> m_vertices;
	BoundingBox m_bbox;

	Animation* m_animation;
};

#endif // ! OBJECT_H