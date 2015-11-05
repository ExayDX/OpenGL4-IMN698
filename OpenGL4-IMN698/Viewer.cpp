//#define GLEW_STATIC

#include "Viewer.h"
#include "ViewerState.h"

//#include "GLM\glm\glm.hpp"
//#include "GLM\glm\gtc\type_ptr.hpp"


#include "Camera.h"
#include "Scene.h"
#include "DefaultTestLevel.h"
#include "SSSSTestLevel.h"
#include "FrameBuffer.h"
#include "ShaderProgram.h"
#include "Quad.h"


#include "glf/glf.h"

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>

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
	m_state->handleKeyboardInput(window, key, scancode, action);
}

void Viewer::window_size_callback_impl(GLFWwindow* window, int width, int height)
{
	glfwSetWindowSize(window, width, height); 
	m_width = width; 
	m_height = height; 
	FrameBuffer::setDimensions(m_width, m_height); 
}

void Viewer::mouse_callback_impl(GLFWwindow* window, double xpos, double ypos)
{	
	m_state->handleMouseMovement(xpos, ypos);
}

void Viewer::mouse_button_callback_impl(GLFWwindow* window, int button, int action, int mods)
{
	m_state->handleMouseClick(window, button, action);
}

void Viewer::scroll_callback_impl(GLFWwindow* window, double xoffset, double yoffset)
{
	m_camera->zoom(yoffset);
}

// Viewer class
void Viewer::moveCameraBetweenFrame(double deltaTime)
{
	if (m_state->getInteractionMode() != LOCKED)
	{
		// Camera controls
		if (m_state->m_keys[GLFW_KEY_W])
			m_camera->move(Camera::CameraDirection::eForward, deltaTime);
		if (m_state->m_keys[GLFW_KEY_S])
			m_camera->move(Camera::CameraDirection::eBackward, deltaTime);
		if (m_state->m_keys[GLFW_KEY_A])
			m_camera->move(Camera::CameraDirection::eLeft, deltaTime);
		if (m_state->m_keys[GLFW_KEY_D])
			m_camera->move(Camera::CameraDirection::eRight, deltaTime);
		if (m_state->m_keys[GLFW_KEY_Z])
			m_camera->zoom(0.001);
		if (m_state->m_keys[GLFW_KEY_X])
			m_camera->zoom(-0.001);
	}
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

	m_window = glfwCreateWindow(m_width, m_height, "Opengl4 Viewer", nullptr, nullptr);
	if (!m_window)
	{
		throw std::runtime_error("Failed to create a GLFW window");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(m_window);
	FrameBuffer::setDimensions(m_width, m_height);
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
Viewer::Viewer() :
	m_camera(nullptr),
	m_currentScene(0)
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

	m_camera = new Camera(&Vec3(0.0f, 1.0f, 0.0f), &Vec3(0.0f, 0.0f, 10.0f), &Vec3(0, 0, 0));
	m_scenes.push_back(new DefaultTestLevel(m_camera));


	m_state = new ViewerState(this);
}

Viewer::~Viewer()
{
	glfwDestroyWindow(m_window);

	if (m_camera)
	{
		delete m_camera;
		m_camera = nullptr; 
	}

	for (int i = 0; i < m_scenes.size(); ++i)
	{
		delete m_scenes[i];
		m_scenes[i] = nullptr;
	}
	
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Viewer::loop()
{
	// Initialize first level
	auto sceneIterator = m_scenes.begin(); 
	assert(sceneIterator != m_scenes.end(), "No scene to render. Please initialize a scene.");

	Scene* currentScene = *sceneIterator;
	currentScene->Initialize();

	m_currentScene = currentScene;

	while (!glfwWindowShouldClose(m_window))
	{
		// Verify if level is still active
		if (currentScene->getLevelIsDone())
		{
			currentScene->sceneTearDown();
			++sceneIterator;
			currentScene = *sceneIterator;
			currentScene->Initialize();
			m_currentScene = currentScene;
		}

		// Get time information
		GLfloat currentFrameTime = glfwGetTime();
		double deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;

		// Check and call events
		glfwPollEvents();
		moveCameraBetweenFrame(deltaTime);

		// Clear the colorbuffer
		glm::mat4 view = m_camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(m_camera->getZoomLevel(), m_width / m_height, 0.1f, 100.0f); 

		// Level drawing
		m_currentScene->setViewMatrix(view);
		m_currentScene->setProjectionMatrix(projection);
		m_currentScene->draw();

		// Swap the buffers
		glfwSwapBuffers(m_window);
	}
}

void Viewer::moveCamera(double xoffset, double yoffset)
{
	m_camera->rotate(xoffset, yoffset);
}
