#ifndef VIEWER_H
#define VIEWER_H

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

private : 
	// Ctors/Dtors
	Viewer();
	~Viewer();

	// Methods
	void createWindow(); 
	void setCallbacks();
	void moveCamera(); 
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
};

#endif

