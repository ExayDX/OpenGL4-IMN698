#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Sphere.h"
#include "Light.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

Scene::Scene()
	: m_levelIsDone(false)
	, m_renderQuad(nullptr)
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

	for (int i = 0; i < m_objects.size(); ++i)
	{
		delete m_objects[i]; m_objects[i] = nullptr; 
	}

	for (int i = 0; i < m_lights.size(); ++i)
	{
		delete m_lights[i]; m_lights[i] = nullptr;
	}
}