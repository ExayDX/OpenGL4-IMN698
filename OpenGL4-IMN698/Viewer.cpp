//#define GLEW_STATIC

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include "GLM\glm\glm.hpp"
#include "GLM\glm\gtc\type_ptr.hpp"

#include "Viewer.h"
#include "Camera.h"
#include "Scene.h"


Viewer* Viewer::m_instance = nullptr; 


// Static Callbacks
static void error_callback(int error, const char* description)
{
	Viewer::getInstance()->error_callback_impl(error, description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Viewer::getInstance()->key_callback_impl(window, key, scancode, action, mods);
}
static void window_size_callback(GLFWwindow* window, int width, int height)
{
	Viewer::getInstance()->window_size_callback_impl(window, width, height);
}
static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	Viewer::getInstance()->mouse_callback_impl(window, xpos, ypos);
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Viewer::getInstance()->mouse_button_callback_impl(window, button, action, mods); 
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Viewer::getInstance()->scroll_callback_impl(window, xoffset, yoffset); 
}

// Viewer's Callbacks
void Viewer::error_callback_impl(int error, const char* description)
{
	fputs(description, stderr);
}

void Viewer::key_callback_impl(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Escape application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	// Wireframe management
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (m_wireFrameEnabled)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			m_wireFrameEnabled = false; 
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			m_wireFrameEnabled = true;
		}
			
	}
	
	// Realtime (key held) interaction management
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			m_keys[key] = true;
		else if (action == GLFW_RELEASE)
			m_keys[key] = false;
	}
}

void Viewer::window_size_callback_impl(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height); 
	m_width = width; 
	m_height = height; 
}

void Viewer::mouse_callback_impl(GLFWwindow* window, double xpos, double ypos)
{	
	if (m_mouseIsClicked)
	{
		if (m_firstClick)
		{
			m_lastMousePosition.x = xpos;
			m_lastMousePosition.y = ypos;
			m_firstClick = false;
		}

		GLfloat xoffset = xpos - m_lastMousePosition.x;
		GLfloat yoffset = m_lastMousePosition.y - ypos; // Y coordinates range from bottom to top

		m_lastMousePosition.x = xpos;
		m_lastMousePosition.y = ypos;

		m_camera->rotate(xoffset, yoffset);
	}
}

void Viewer::mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		m_mouseIsClicked = true;
		m_firstClick = true;
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		m_mouseIsClicked = false; 
}

void Viewer::scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset)
{
	m_camera->zoom(yoffset);
}

// Viewer class
void Viewer::moveCamera()
{
	// Camera controls
	if (m_keys[GLFW_KEY_W])
		m_camera->move(Camera::CameraDirection::eForward, m_deltaTime);
	if (m_keys[GLFW_KEY_S])
		m_camera->move(Camera::CameraDirection::eBackward, m_deltaTime);
	if (m_keys[GLFW_KEY_A])
		m_camera->move(Camera::CameraDirection::eLeft, m_deltaTime);
	if (m_keys[GLFW_KEY_D])
		m_camera->move(Camera::CameraDirection::eRight, m_deltaTime);
	if (m_keys[GLFW_KEY_Z])
		m_camera->zoom(0.001);
	if (m_keys[GLFW_KEY_X])
		m_camera->zoom(-0.001);
}

Viewer* Viewer::getInstance()
{
	if (!m_instance)
		m_instance = new Viewer(); 

	return m_instance; 
}

void Viewer::Destroy()
{
	delete m_instance;
	m_instance = nullptr; 
}

void Viewer::setCallbacks()
{
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, window_size_callback);
	glfwSetCursorPosCallback(m_window, mouse_callback);
	glfwSetMouseButtonCallback(m_window, mouse_button_callback);
}

/*
* @brief : Manage window's creation and getting important monitor settings. 
*/
void Viewer::createWindow()
{
	GLFWmonitor* monitor = glfwGetPrimaryMonitor(); 
	const GLFWvidmode* mode = glfwGetVideoMode(monitor); 

	// Window's size
	m_width = mode->width;
	m_height = mode->height; 

	// Window's hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE) //Mac OSX

	m_window = glfwCreateWindow(m_width, m_height, "Simple example", nullptr, nullptr);
	if (!m_window)
	{
		throw std::runtime_error("Failed to create a GLFW window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window);
}

void Viewer::setupViewport()
{
	// Create Viewport
	glViewport(0, 0, m_width, m_height);

	// Set GL options
	glEnable(GL_DEPTH_TEST);
}

/*
* @brief : Acts as the main in a non-object oriented OpenGL program.  
*/
Viewer::Viewer()
	: m_lastMousePosition(0, 0)
	, m_firstClick(true)
	, m_camera(nullptr)
	, m_scene(nullptr)
	, m_wireFrameEnabled(false)
	, m_mouseIsClicked(false)
{
	// GLFW initialization
	if (!glfwInit())
	{
		throw std::runtime_error("glfwInit failed");
		exit(EXIT_FAILURE);
	}
	
	createWindow();
	setCallbacks();
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// GLEW initialization
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
		exit(EXIT_FAILURE);
	}

	setupViewport(); 
	glfwSwapInterval(1);

	m_camera = new Camera(&glm::vec3(0.0f, 1.0f, 0.0f), &glm::vec3(0.0f, 0.0f, 10.0f), &glm::vec3(0, 0, 0));
	m_scene = new Scene();
}

Viewer::~Viewer()
{
	glfwDestroyWindow(m_window);

	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr; 
	}

	if (m_scene)
	{
		delete m_scene;
		m_scene = nullptr;
	}
	
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


void Viewer::loop()
{
	while (!glfwWindowShouldClose(m_window))
	{
		GLfloat currentFrameTime = glfwGetTime();
		m_deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;

		// Check and call events
		glfwPollEvents();
		moveCamera();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Coordinate system matrices 
		glm::mat4 view = m_camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(m_camera->getZoomLevel(), m_width / m_height, 0.1f, 100.0f); 

		m_scene->setViewMatrix(view);
		m_scene->setProjectionMatrix(projection); 

		m_scene->draw(); 

		// Swap the buffers
		glfwSwapBuffers(m_window);
	}
}