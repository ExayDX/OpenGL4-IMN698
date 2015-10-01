#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>

// Forward Declaration
class Object; 
class ShaderProgram;

class Scene
{
public : 
	Scene();
	~Scene();

	void draw(); 
	GLuint getAShaderProgramId(std::string shaderName); 

private : 

	std::vector<Object*> m_objects;
	std::map<std::string, ShaderProgram*> m_shaderPrograms; 
	void levelSetup();
	void levelTearDown();
	void createShaderPrograms();
	void setupLight();

};

#endif