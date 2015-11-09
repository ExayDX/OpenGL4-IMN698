#ifndef VIEWER_H
#define VIEWER_H

#include "Types.h"
#include "Camera.h"
#include "ConsoleListener.h"
#include "Scene.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"

#include <vector>

// Forward declarations
class Camera;
class Scene; 
class ViewerState;

class Viewer
{
	static const int FRAME_PER_SECOND = 24;
public : 

	void error_callback_impl(int error, const char* description);
	void key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mods);
	void window_size_callback_impl(GLFWwindow* window, int width, int height);
	void mouse_callback_impl(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods);
	void scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset); 

	static Viewer* getInstance();
	void Destroy();
	
	void loop();

	// Getters
	glm::mat4 getViewMatrix(){ return m_viewMatrix; }
	glm::mat4 getProjectionMatrix(){ return m_projectionMatrix; }
	Camera* getCamera(){ return m_camera; }
	Scene*  getCurrentScene() { return m_currentScene; }

	void setDrawLight(bool val) { m_currentScene->drawAllLights(val); }

	//interface between consoleListener and Scene
	void loadModel(const std::string& path, Vec3 position, std::string shaderProgram);

private : 
	// Ctors/Dtors
	Viewer();
	~Viewer();

	// Methods
	void createWindow(); 
	void setCallbacks();
	void moveCameraBetweenFrame(double timeBetweenFrame);
	void moveCamera(double xoffset, double yoffset);
	void setupViewport(); 

	// Instance
	static Viewer* m_instance; 

	// Variables
	// -- State
	ViewerState* m_state; // HACK : This is not a proper Object Oriented State and it Operations should be handled by classes implementing different states...

	// -- Display variables
	GLFWwindow* m_window;
	Camera* m_camera; 

	GLfloat m_width;
	GLfloat m_height;
	GLboolean m_viewingIsOver;

	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix; 

	std::vector<Scene*> m_scenes;
	Scene* m_currentScene; 

	// -- Time and general computation variables
	GLfloat m_deltaTime;
	GLfloat m_lastFrameTime;

	ConsoleListener* m_listener;

	glm::vec2 m_lastMousePosition;

	int m_lastFrame;
};

#endif

