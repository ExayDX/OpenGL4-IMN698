#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include "glm/glm/glm.hpp"
#include <boost/filesystem.hpp>

#include "Shader.h"

// Forward Declaration
class Object; 
class Light; 
class ShaderProgram;
class Material; 

class Scene
{
public : 
	Scene();
	~Scene();

	void draw(); 

	GLuint getAShaderProgramId(std::string shaderName); 
	void setViewMatrix(const glm::mat4& aViewMatrix){ m_viewMatrix = aViewMatrix; }
	void setProjectionMatrix(const glm::mat4& aProjectionMatrix){ m_projectionMatrix = aProjectionMatrix; }

	void changeShaderProgramName(int car);

private:
	static Shader* createShader(const boost::filesystem::path& path);
	void levelSetup();
	void lightSetup();
	void levelTearDown();
	void createShaderPrograms();
	void createMaterials();
private : 

	std::vector<Object*> m_objects;
	std::vector<Light*> m_lights; 
	std::map<std::string, ShaderProgram*> m_shaderPrograms;
	std::map<std::string, Material*> m_materials; 

	glm::mat4 m_viewMatrix; 
	glm::mat4 m_projectionMatrix; 

	std::string m_shaderProgramName;

	//float m_ambientLightingStrength; 
	//glm::vec3 ambient; 



};

#endif