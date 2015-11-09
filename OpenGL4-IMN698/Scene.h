#ifndef SCENE_H
#define SCENE_H

#include "Types.h"
#include "Camera.h"

#include <gl/glew.h>
#include "glm/glm/glm.hpp"

#include <vector>
#include <queue>
#include <map>

#include <mutex>

// Forward Declaration
class Object; 
class Light; 
class ShaderProgram;
class Material; 
class Quad; 
class FrameBuffer; 
class Camera;

struct ObjectPending
{
	ObjectPending(Vec3 pos, std::string path, std::string shaderProgram) :
		m_position(pos),
		m_path(path),
		m_shaderProgram(shaderProgram)
	{	}

	Vec3 m_position;
	std::string m_path;
	std::string m_shaderProgram;
};

class Scene
{

public : 
	Scene();
	~Scene();

	virtual void Initialize(); 
	virtual void sceneTearDown();

	virtual void draw(int currentFrame) = 0;

	virtual void setViewMatrix(const glm::mat4& aViewMatrix){ m_viewMatrix = aViewMatrix; }
	virtual void setProjectionMatrix(const glm::mat4& aProjectionMatrix){ m_projectionMatrix = aProjectionMatrix; }
	virtual bool getLevelIsDone(){ return m_levelIsDone; }
	virtual std::vector<Object*> getObjects(){ return m_objects; }
	virtual std::vector<Light*> getLights(){ return m_lights; }

	virtual void drawAllLights(bool allLightsAreDrawn);
	glm::mat4 getProjectionMatrix() { return m_projectionMatrix; }
	glm::mat4 getViewMatrix() { return m_viewMatrix; }

	std::vector<std::string> getShaderList() const;

	void loadModel(const std::string& path, Vec3 position, std::string shaderProgram);
	void loadPendingModels();

protected:
	// Methods
	virtual void levelSetup() = 0;
	virtual void lightSetup() = 0;
	virtual void buffersSetup() = 0;
	virtual void createShaderPrograms() = 0;
	virtual void createMaterials() = 0;
	
protected:

	std::vector<Object*> m_objects;
	std::queue<ObjectPending> m_objectsToBeCreated;
	std::vector<Light*> m_lights;
	std::map<std::string, ShaderProgram*> m_shaderPrograms;
	std::map<std::string, Material*> m_materials;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;

	bool m_levelIsDone;
	bool m_allLightsAreDrawn; 
	Quad* m_renderQuad;

	std::map<std::string, FrameBuffer*> m_frameBuffers;

	std::mutex m_objectVectorMutex;
};

#endif