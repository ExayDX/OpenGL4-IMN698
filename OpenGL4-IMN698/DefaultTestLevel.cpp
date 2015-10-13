#include "DefaultTestLevel.h"

#include "Sphere.h"
#include "Light.h"

#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"
#include "GLM/glm/gtc/type_ptr.hpp"

DefaultTestLevel::DefaultTestLevel()
	: Scene()
{

}

// LOOP
void DefaultTestLevel::draw()
{
	for each (Object* obj in m_objects)
	{
		GLuint shaderProgramID = obj->getShaderProgramId();
		glUseProgram(shaderProgramID);

		// Compute/Get information to pass to uniforms
		glm::mat4 modelMatrix = obj->getModelMatrix();
		glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(m_viewMatrix * modelMatrix)));
		const Material* objectMaterial = obj->getMaterial();
		const Material* lightMaterial0 = m_lights[0]->getMaterial();
		const Material* lightMaterial1 = m_lights[1]->getMaterial();
		glm::vec3 lightPosition0 = m_lights[0]->getPosition();
		glm::vec3 lightPosition1 = m_lights[1]->getPosition();
		Light::AttenuationProperties lightProperties0 = m_lights[0]->getAttenuationProperties();
		Light::AttenuationProperties lightProperties1 = m_lights[1]->getAttenuationProperties();

		// Get Uniforms location
		// Display matrixes
		GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
		GLuint viewLoc = glGetUniformLocation(shaderProgramID, "view");
		GLuint projectionLoc = glGetUniformLocation(shaderProgramID, "projection");
		GLuint normalMatrixLoc = glGetUniformLocation(shaderProgramID, "normalMatrix");
		GLuint nbLightLoc = glGetUniformLocation(shaderProgramID, "nbLights");

		// Object information
		GLuint objectColorLoc = glGetUniformLocation(shaderProgramID, "objectColor");
		GLuint objectAmbientLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.ambientCoefs");
		GLuint objectDiffuseLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.diffuseCoefs");
		GLuint objectSpecularLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.specularCoefs");
		GLuint objectShininessLoc = glGetUniformLocation(shaderProgramID, "objectMaterial.shininess");

		// Light0 information
		GLuint light0PositionLoc = glGetUniformLocation(shaderProgramID, "lightsPositions[0]");
		GLuint light0AmbientLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[0].material.ambientCoefs");
		GLuint light0DiffuseLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[0].material.diffuseCoefs");
		GLuint light0SpecularLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[0].material.specularCoefs");
		GLuint light0ShininessLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[0].material.shininess");
		GLuint light0AttConstant = glGetUniformLocation(shaderProgramID, "lightsProperties[0].constant");
		GLuint light0AttLinear = glGetUniformLocation(shaderProgramID, "lightsProperties[0].linear");
		GLuint light0AttQuadratic = glGetUniformLocation(shaderProgramID, "lightsProperties[0].quadratic");

		// Light1 information
		GLuint light1PositionLoc = glGetUniformLocation(shaderProgramID, "lightsPositions[1]");
		GLuint light1AmbientLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[1].material.ambientCoefs");
		GLuint light1DiffuseLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[1].material.diffuseCoefs");
		GLuint light1SpecularLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[1].material.specularCoefs");
		GLuint light1ShininessLoc = glGetUniformLocation(shaderProgramID, "lightsProperties[1].material.shininess");
		GLuint light1AttConstant = glGetUniformLocation(shaderProgramID, "lightsProperties[1].constant");
		GLuint light1AttLinear = glGetUniformLocation(shaderProgramID, "lightsProperties[1].linear");
		GLuint light1AttQuadratic = glGetUniformLocation(shaderProgramID, "lightsProperties[1].quadratic");

		// Assign Uniforms Values
		// Display Matrixes 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_viewMatrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_projectionMatrix));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glUniform1f(nbLightLoc, m_lights.size());

		// Object information 
		glUniform3f(objectAmbientLoc, objectMaterial->m_ambientCoefs.x, objectMaterial->m_ambientCoefs.y, objectMaterial->m_ambientCoefs.z);
		glUniform3f(objectDiffuseLoc, objectMaterial->m_diffuseCoefs.x, objectMaterial->m_diffuseCoefs.y, objectMaterial->m_diffuseCoefs.z);
		glUniform3f(objectSpecularLoc, objectMaterial->m_specularCoefs.x, objectMaterial->m_specularCoefs.y, objectMaterial->m_specularCoefs.z);
		glUniform1f(objectShininessLoc, objectMaterial->m_shininess);

		// Light0 information
		glUniform3f(light0PositionLoc, lightPosition0.x, lightPosition0.y, lightPosition0.z);
		glUniform3f(light0AmbientLoc, lightMaterial0->m_ambientCoefs.x, lightMaterial0->m_ambientCoefs.y, lightMaterial0->m_ambientCoefs.z);
		glUniform3f(light0DiffuseLoc, lightMaterial0->m_diffuseCoefs.x, lightMaterial0->m_diffuseCoefs.y, lightMaterial0->m_diffuseCoefs.z);
		glUniform3f(light0SpecularLoc, lightMaterial0->m_specularCoefs.x, lightMaterial0->m_specularCoefs.y, lightMaterial0->m_specularCoefs.z);
		glUniform1f(light0ShininessLoc, lightMaterial0->m_shininess);
		glUniform1f(light0AttConstant, lightProperties0.m_constant);
		glUniform1f(light0AttLinear, lightProperties0.m_linear);
		glUniform1f(light0AttQuadratic, lightProperties0.m_quadratic);

		// Light1 information
		glUniform3f(light1PositionLoc, lightPosition1.x, lightPosition1.y, lightPosition1.z);
		glUniform3f(light1AmbientLoc, lightMaterial1->m_ambientCoefs.x, lightMaterial1->m_ambientCoefs.y, lightMaterial1->m_ambientCoefs.z);
		glUniform3f(light1DiffuseLoc, lightMaterial1->m_diffuseCoefs.x, lightMaterial1->m_diffuseCoefs.y, lightMaterial1->m_diffuseCoefs.z);
		glUniform3f(light1SpecularLoc, lightMaterial1->m_specularCoefs.x, lightMaterial1->m_specularCoefs.y, lightMaterial1->m_specularCoefs.z);
		glUniform1f(light1ShininessLoc, lightMaterial1->m_shininess);
		glUniform1f(light1AttConstant, lightProperties1.m_constant);
		glUniform1f(light1AttLinear, lightProperties1.m_linear);
		glUniform1f(light1AttQuadratic, lightProperties1.m_quadratic);

		obj->draw();
	}
}

void DefaultTestLevel::createShaderPrograms()
{
	// Create Shader programs
	ShaderProgram* defaultShaderProgram = new ShaderProgram("defaultVS.glsl", "defaultFS.glsl");
	ShaderProgram* phongShaderProgram = new ShaderProgram("Phong.vs", "Phong.fg");
	ShaderProgram* SSSProgram = new ShaderProgram("SSS.vs", "SSS.fg");

	// Insert ShaderProgram in the list
	m_shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("default", defaultShaderProgram));
	m_shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("phong", phongShaderProgram));
	m_shaderPrograms.insert(std::pair<std::string, ShaderProgram*>("SSS", SSSProgram));
}

void DefaultTestLevel::createMaterials()
{
	Material* defaultMaterial = new Material(glm::vec3(0.2f, 0.1f, 0.05f),
		glm::vec3(0.8f, 0.4f, 0.31f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		32.0f);

	Material* blueMaterial = new Material(glm::vec3(0.0f, 0.1f, 0.06f),
		glm::vec3(0.0f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		128.0f);

	Material* orangeMaterial = new Material(glm::vec3(1.0f, 0.6f, 0.0f),
		glm::vec3(1.0f, 0.6f, 0.0f),
		glm::vec3(1.0f, 1.0f, 1.0f),
		32);

	Material* defaultLightMaterial = new Material(glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		1.0f);

	Material* blackMaterial = new Material(glm::vec3(0.0f),
		glm::vec3(0.0f),
		glm::vec3(0.0f),
		0.0f);

	m_materials.insert(std::pair<std::string, Material*>("default", defaultMaterial));
	m_materials.insert(std::pair<std::string, Material*>("blue", blueMaterial));
	m_materials.insert(std::pair<std::string, Material*>("orange", orangeMaterial));
	m_materials.insert(std::pair<std::string, Material*>("defaultLight", defaultLightMaterial));
	m_materials.insert(std::pair<std::string, Material*>("black", blackMaterial));
}

void DefaultTestLevel::levelSetup()
{

	Object* sphere1 = new Sphere(glm::vec3(-7, 0, 0), m_materials["default"], 2, 40, 40, m_shaderPrograms["phong"]->getId());
	m_objects.push_back(sphere1);

	Object* sphere2 = new Sphere(glm::vec3(0, 0, 0), m_materials["orange"], 2, 40, 40, m_shaderPrograms["default"]->getId());
	m_objects.push_back(sphere2);

	Object* sphere3 = new Sphere(glm::vec3(7, 0, 0), m_materials["blue"], 2, 40, 40, m_shaderPrograms["SSS"]->getId());
	m_objects.push_back(sphere3);

}

void DefaultTestLevel::lightSetup()
{
	Light::AttenuationProperties attenuationProp;
	attenuationProp.m_constant = 1.0f;
	attenuationProp.m_linear = 0.02f;
	attenuationProp.m_quadratic = 0.0005f;

	Light* light1 = new Light(glm::vec3(0, 10, 0), m_materials["defaultLight"], attenuationProp);
	m_lights.push_back(light1);

	Light* light2 = new Light(glm::vec3(-30, 40, 0), m_materials["defaultLight"], attenuationProp);
	m_lights.push_back(light2);
}
