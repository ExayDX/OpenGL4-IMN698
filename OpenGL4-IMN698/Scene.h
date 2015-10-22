#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include "glm/glm/glm.hpp"
#include "GL/glew.h"
#include "RenderBuffer.h"

// Forward Declaration
class Object; 
class Light; 
class ShaderProgram;
class Material; 
class Quad; 

class Scene
{
public : 
	Scene();
	~Scene();

	virtual void Initialize(); 
	virtual void sceneTearDown();
	virtual void draw() = 0;

	virtual void setViewMatrix(const glm::mat4& aViewMatrix){ m_viewMatrix = aViewMatrix; }
	virtual void setProjectionMatrix(const glm::mat4& aProjectionMatrix){ m_projectionMatrix = aProjectionMatrix; }
	virtual bool getLevelIsDone(){ return m_levelIsDone; }

protected : 

	std::vector<Object*> m_objects;
	std::vector<Light*> m_lights; 
	std::map<std::string, ShaderProgram*> m_shaderPrograms;
	std::map<std::string, Material*> m_materials; 

	glm::mat4 m_viewMatrix; 
	glm::mat4 m_projectionMatrix; 

	bool m_levelIsDone;

	Quad* m_renderQuad; 

	// Buffers
	GLuint m_hdrFBO;
	std::map<RenderBuffer::BufferType, std::vector<GLuint>* > m_renderBuffers;

	// Methods
	virtual void levelSetup() = 0;
	virtual void lightSetup() = 0; 
	virtual void buffersSetup() = 0;
	virtual void createShaderPrograms() = 0;
	virtual void createMaterials() = 0; 
};

#endif