#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include "Scene.h"
#include "Sphere.h"
#include "Light.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

Scene::Scene()
{
	createShaderPrograms();
	createMaterials(); 
	levelSetup();
	lightSetup(); 
}

Scene::~Scene()
{
	levelTearDown(); 
}

void Scene::createShaderPrograms()
{
	// Create Shader programs
	ShaderProgram* defaultShaderProgram = new ShaderProgram("defaultVS.glsl", "defaultFS.glsl");
	ShaderProgram* phongShaderProgram = new ShaderProgram("Phong.vs", "Phong.fg");

	// Insert ShaderProgram in the list
	m_shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("default", defaultShaderProgram));
	m_shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("phong", phongShaderProgram)); 
}

void Scene::createMaterials()
{
	Material* defaultMaterial =		 new Material(glm::vec3(0.2f, 0.1f, 0.05f),
												  glm::vec3(0.8f, 0.4f, 0.31f),
												  glm::vec3(0.5f, 0.5f, 0.5f),
												  32.0f);

	Material* blueMaterial =		 new Material(glm::vec3(0.0f, 0.1f, 0.06f),
												  glm::vec3(0.0f, 0.5f, 0.5f),
												  glm::vec3(0.5f, 0.5f, 0.5f),
												  128.0f);

	Material* orangeMaterial =		 new Material(glm::vec3(1.0f, 0.6f, 0.0f),
												  glm::vec3(1.0f, 0.6f, 0.0f),
												  glm::vec3(1.0f, 1.0f, 1.0f),
												  32); 
	
	Material* defaultLightMaterial = new Material(glm::vec3(1.0f), 
												  glm::vec3(1.0f),
												  glm::vec3(1.0f),
												  1.0f); 

	m_materials.insert(std::pair<std::string, Material*>("default", defaultMaterial)); 
	m_materials.insert(std::pair<std::string, Material*>("blue", blueMaterial)); 
	m_materials.insert(std::pair<std::string, Material*>("orange", orangeMaterial)); 
	m_materials.insert(std::pair<std::string, Material*>("defaultLight", defaultLightMaterial));

}

void Scene::levelSetup()
{	

	Object* sphere1 = new Sphere(glm::vec3(-7, 0, 0), m_materials["default"], 2, 40, 40, m_shaderPrograms["phong"]->getId());
	m_objects.push_back(sphere1);

	Object* sphere2 = new Sphere(glm::vec3(0, 0, 0), m_materials["orange"], 2, 40, 40, m_shaderPrograms["default"]->getId()); 
	m_objects.push_back(sphere2);

	Object* sphere3 = new Sphere(glm::vec3(7, 0, 0), m_materials["blue"], 2, 40, 40, m_shaderPrograms["phong"]->getId());
	m_objects.push_back(sphere3);

}

void Scene::levelTearDown()
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

// LOOP
void Scene::draw()
{	
	for each (Object* obj in m_objects)
	{
		GLuint shaderProgramID = obj->getShaderProgramId(); 
		glUseProgram(shaderProgramID);

		// Compute/Get information to pass to uniforms
		glm::mat4 modelMatrix = obj->getModelMatrix();
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_viewMatrix * modelMatrix)));
		const Material* objectMaterial = obj->getMaterial();
		const Material* lightMaterial = m_lights[0]->getMaterial();
		glm::vec3 lightPosition = m_lights[0]->getPosition();

		// Get Uniforms location
		GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
		GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
		GLuint projectionLoc = glGetUniformLocation(shaderProgramID, "projection");
		GLuint normalMatrixLoc = glGetUniformLocation(shaderProgramID, "normalMatrix");
		GLuint objectColorLoc = glGetUniformLocation(shaderProgramID, "objectColor");
		GLuint lightColorLoc = glGetUniformLocation(shaderProgramID, "lightColor");
		GLuint lightPositionLoc = glGetUniformLocation(shaderProgramID, "lightPosition");
		GLuint objectAmbientLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.ambientCoefs"); 
		GLuint objectDiffuseLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.diffuseCoefs");
		GLuint objectSpecularLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.specularCoefs");
		GLuint objectShininessLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.shininess");
		GLuint lightAmbientLoc = glGetUniformLocation(shaderProgramID, "lightMaterial.ambientCoefs");
		GLuint lightDiffuseLoc = glGetUniformLocation(shaderProgramID, "lightMaterial.diffuseCoefs");
		GLuint lightSpecularLoc = glGetUniformLocation(shaderProgramID, "lightMaterial.specularCoefs");
		GLuint lightShininessLoc = glGetUniformLocation(shaderProgramID, "lightMaterial.shininess");

		// Assign Uniforms Values
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix)); 
		glUniform3f(lightPositionLoc, lightPosition.x, lightPosition.y, lightPosition.z); 
		glUniform3f(objectAmbientLoc, objectMaterial->m_ambientCoefs.x, objectMaterial->m_ambientCoefs.y, objectMaterial->m_ambientCoefs.z);
		glUniform3f(objectDiffuseLoc, objectMaterial->m_diffuseCoefs.x, objectMaterial->m_diffuseCoefs.y, objectMaterial->m_diffuseCoefs.z);
		glUniform3f(objectSpecularLoc, objectMaterial->m_specularCoefs.x, objectMaterial->m_specularCoefs.y, objectMaterial->m_specularCoefs.z);
		glUniform1f(objectShininessLoc, 32.0f); 
		glUniform3f(lightAmbientLoc, lightMaterial->m_ambientCoefs.x, lightMaterial->m_ambientCoefs.y, lightMaterial->m_ambientCoefs.z);
		glUniform3f(lightDiffuseLoc, lightMaterial->m_diffuseCoefs.x, lightMaterial->m_diffuseCoefs.y, lightMaterial->m_diffuseCoefs.z);
		glUniform3f(lightSpecularLoc, lightMaterial->m_specularCoefs.x, lightMaterial->m_specularCoefs.y, lightMaterial->m_specularCoefs.z);
		glUniform1f(lightShininessLoc, 32.0f);

		obj->draw();
	}
}

void Scene::lightSetup()
{
	Light* light1 = new Light(glm::vec3(0, 10, 0), m_materials["defaultLight"]); 
	m_lights.push_back(light1); 
}
