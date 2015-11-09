
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

Scene::Scene()
	: m_levelIsDone(false)
	, m_renderQuad(nullptr)
{
}

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
		for each (Object* obj in m_objects)
		{
			delete m_renderQuad; m_renderQuad = nullptr;
		}
	}
}

void Scene::drawAllLights(bool allLightsAreDrawn)
{
	m_allLightsAreDrawn = allLightsAreDrawn;
	for each (auto light in m_lights)
	{
		light->setVisible(allLightsAreDrawn);
	}
}

void Scene::loadModel(const std::string& path, Vec3 position, std::string shaderProgram)
{
	ObjectPending obj(position, path, shaderProgram);
	m_objectsToBeCreated.push(obj);
}

void Scene::loadPendingModels()
{
	while (!m_objectsToBeCreated.empty())
	{
		ObjectPending obj = m_objectsToBeCreated.front();
		Object* model1 = ModelLoader::loadModel(obj.m_path, m_materials["default"], m_shaderPrograms[obj.m_shaderProgram]->getId());
		model1->setVisible(true);
		model1->translate(obj.m_position);
		std::lock_guard<std::mutex> lock(m_objectVectorMutex);
		m_objects.push_back(model1);
		m_objectsToBeCreated.pop();
	}

}

std::vector<std::string> Scene::getShaderList() const
{
	std::vector<std::string> shaders;

	for (auto i = m_shaderPrograms.begin(); i != m_shaderPrograms.end(); ++i)
	{
		shaders.push_back(i->first);
	}
	return shaders;
}

void Scene::preDraw()
{
	loadPendingModels();
	m_objectVectorMutex.lock();
}

void Scene::postDraw()
{
	m_objectVectorMutex.unlock();
}