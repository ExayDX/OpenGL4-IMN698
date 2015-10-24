#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Light.h"
#include "ModelLoader.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

Scene::Scene(Camera* cam)
	: m_levelIsDone(false)
	, m_renderQuad(nullptr)
	, m_camera(cam)
{

}

//Scene::Scene(Camera* camera):
//	m_camera(camera)

Scene::~Scene()
{
	sceneTearDown(); 
}

void Scene::Initialize()
{
	createShaderPrograms();
	createMaterials();
	buffersSetup();
	levelSetup();
	lightSetup();
}

void Scene::sceneTearDown()
{
	for (auto it = m_shaderPrograms.begin(); it != m_shaderPrograms.end(); ++it)
	{
		delete it->second; it->second = nullptr; 
	}
	m_shaderPrograms.clear();

	for (auto it = m_materials.begin(); it != m_materials.end(); ++it)
	{
		delete it->second; it->second = nullptr;
	}
	m_materials.clear();

	for (auto it = m_frameBuffers.begin(); it != m_frameBuffers.end(); ++it)
	{
		delete it->second; it->second = nullptr; 
	}

	for (int i = 0; i < m_objects.size(); ++i)
	{
		delete m_objects[i]; m_objects[i] = nullptr; 
	}

	for (int i = 0; i < m_lights.size(); ++i)
	{
		delete m_lights[i]; m_lights[i] = nullptr;
	}

	if (m_renderQuad)
	{
		delete m_renderQuad; m_renderQuad = nullptr; 
	}
}

void Scene::setDrawLight(bool val)
{
	for each (Light* light in m_lights)
	{
		light->setVisible(val);
	}
}

std::vector<Object*> Scene::getObjectsAndLights() const
{
	std::vector<Object*> objs(m_objects);
	for each (Light* light in m_lights)
	{
		objs.push_back(light);
	}
	return objs;
}