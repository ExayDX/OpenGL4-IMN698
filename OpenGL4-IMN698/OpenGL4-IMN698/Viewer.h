#ifndef VIEWER_H
#define VIEWER_H

#include <GLFW/glfw3.h>
#include "GLM/glm/glm.hpp"
#include "GLM/glm/gtc/matrix_transform.hpp"

// Forward declarations
class Camera;
class Scene; 


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

private : 
	// Ctors/Dtors
	Viewer();
	~Viewer();

	void createWindow(); 
	void setCallbacks();
	void moveCamera(); 
	void setupViewport(); 

	static Viewer* m_instance; 
	GLFWwindow* m_window;
	Camera* m_camera; 
	Scene* m_scene; 
	bool m_keys[1024]; // All the keys that can be pressed

	glm::vec2 m_lastMousePosition;
	GLfloat m_deltaTime; 
	GLfloat m_lastFrameTime; 
	bool m_firstClick; 
	GLfloat m_width;
	GLfloat m_height;
	bool m_wireFrameEnabled; 
	bool m_mouseIsClicked; 
};

#endif

