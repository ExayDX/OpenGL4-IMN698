#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Sphere.h"
#include "FragmentShader.h"
#include "VertexShader.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

Scene::Scene()
{
	createShaderPrograms();
	levelSetup();
}

Scene::~Scene()
{
	levelTearDown(); 
}

void Scene::createShaderPrograms()
{
	// Create Default Shader Program
	VertexShader defaultVertexShader("defaultVS.glsl");
	FragmentShader defaultFragmentShader("defaultFS.glsl");
	ShaderProgram* defaultShaderProgram = new ShaderProgram(&defaultVertexShader, &defaultFragmentShader);

	GLuint modelLocation = glGetUniformLocation(defaultShaderProgram->getProgramId(), "model");
	defaultShaderProgram->insertNewShaderParameterLocation("model", modelLocation); 

	// Create Phong Shader Program
	VertexShader phongVertexShader("Phong.vs");
	FragmentShader phongFragmentShader("Phong.fg");
	ShaderProgram* phongShaderProgram = new ShaderProgram(&phongVertexShader, &phongFragmentShader);

	// Insert ShaderProgram in the list
	m_shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("default", defaultShaderProgram));
	m_shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("phong", phongShaderProgram)); 
}

void Scene::levelSetup()
{
	Object* sphere1 = new Sphere(glm::vec3(0, 0, 0), glm::vec3(1, 0, 0), 2, 40, 40, m_shaderPrograms["default"]); 
	sphere1->setupObject();
	m_objects.push_back(sphere1);

	Object* sphere2 = new Sphere(glm::vec3(-7, 0, 0), glm::vec3(1, 0, 0), 2, 40, 40, m_shaderPrograms["default"]);
	sphere2->setupObject(); 
	m_objects.push_back(sphere2);

	Object* sphere3 = new Sphere(glm::vec3(7, 0, 0), glm::vec3(1, 0, 0), 2, 40, 40, m_shaderPrograms["default"]);
	sphere3->setupObject(); 
	m_objects.push_back(sphere3);

	glBindVertexArray(0);
}

void Scene::levelTearDown()
{
	for (auto it = m_shaderPrograms.begin(); it != m_shaderPrograms.end(); ++it)
	{
		delete it->second; it->second = nullptr; 
	}
	m_shaderPrograms.clear();

	for (int i = 0; i < m_objects.size(); ++i)
	{
		delete m_objects[i]; m_objects[i] = nullptr; 
	}
}

// LOOP
void Scene::draw()
{	
	for each (Object* obj in m_objects)
	{
		// TODO : Move these calls to a shaderprogram (cuz it's shaderprog dependant)
		GLuint modelLoc = glGetUniformLocation(obj->getShaderProgramId(), "model");
		
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(obj->getModelMatrix()));


		obj->draw();
	}
}

GLuint Scene::getAShaderProgramId(std::string shaderName)
{
	return m_shaderPrograms[shaderName]->getProgramId(); 
}

void Scene::setupLight()
{
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat diffuse[] = { 1, 1, 1, 1.0 };
	GLfloat ambient[] = { 1, 1, 1, 1.0 };
	GLfloat specular[] = { 1, 1, 1, 1.0 };

	GLfloat pos[] = { 0, 0, 1, 1.0 };

	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glLightfv(GL_LIGHT0, GL_POSITION, pos);
}
