
#include "Viewer.h"
#include "ViewerState.h"
#include "Camera.h"
#include "Scene.h"
#include "DefaultTestLevel.h"
#include "SSSSTestLevel.h"
#include "FrameBuffer.h"
#include "ShaderProgram.h"
#include "Quad.h"
#include "ModelContainer.h"

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
	// Mode management according to key entered (Managed by Viewer's viewerState)
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
// According to how the viewerstate is currently made, shouldn't this be in viewerstate?
void Viewer::moveCameraBetweenFrame(double deltaTime)
{
	if (m_state->getInteractionMode() != LOCKED)
	{
		// Camera controls
		if (m_state->getKeys()[GLFW_KEY_W])
			m_camera->move(Camera::CameraDirection::eForward, deltaTime);
		if (m_state->getKeys()[GLFW_KEY_S])
			m_camera->move(Camera::CameraDirection::eBackward, deltaTime);
		if (m_state->getKeys()[GLFW_KEY_A])
			m_camera->move(Camera::CameraDirection::eLeft, deltaTime);
		if (m_state->getKeys()[GLFW_KEY_D])
			m_camera->move(Camera::CameraDirection::eRight, deltaTime);
		if (m_state->getKeys()[GLFW_KEY_Z])
			m_camera->zoom(0.001);
		if (m_state->getKeys()[GLFW_KEY_X])
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
	glDepthFunc(GL_LESS);
}

/*
* @brief : Acts as the main in a non-object oriented OpenGL program.  
*/
Viewer::Viewer()
	: m_camera(nullptr)
	, m_viewingIsOver(false)
	, m_currentScene(nullptr)
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

	m_state = new ViewerState();
	m_camera = new Camera(&glm::vec3(0.0f, 1.0f, 0.0f), &glm::vec3(0.0f, 0.0f, 10.0f), &glm::vec3(0, 0, 0));
	m_scenes.push_back(new DefaultTestLevel()); 
	m_listener = new ConsoleListener();

	//start animation once scene is ready
	m_currentFrame = 0;
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
	
	delete m_state; m_state = nullptr; 
	delete m_listener; m_listener = nullptr; 

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Viewer::loop()
{
	// Initialize first level
	auto sceneIterator = m_scenes.begin(); 
	assert(sceneIterator != m_scenes.end(), "No scene to render. Please initialize a scene.");

	m_currentScene = *sceneIterator;
	m_currentScene->Initialize();

	m_lastTime = Clock::now();

	while (!glfwWindowShouldClose(m_window) && !m_viewingIsOver)
	{
		// Verify if level is still active
		if (m_currentScene->getLevelIsDone())
		{
			m_currentScene->sceneTearDown();
			++sceneIterator;

			if (sceneIterator != m_scenes.end())
			{
				m_currentScene = *sceneIterator;
				m_currentScene->Initialize();
			}
			else
			{
				m_viewingIsOver = true; 
			}
		}

		glm::mat4 projection = glm::perspective(m_camera->getZoomLevel(), m_width / m_height, 0.1f, 100.0f);
		m_currentScene->setProjectionMatrix(projection);

		// Timing Calculations
		GLfloat currentFrameTime = glfwGetTime();
		double deltaTime = currentFrameTime - m_lastFrameTime;
		m_lastFrameTime = currentFrameTime;

		// Check and call events
		glfwPollEvents();
		moveCameraBetweenFrame(deltaTime);

		// Update Matrix
		m_viewMatrix = m_camera->GetViewMatrix();
		m_projectionMatrix = glm::perspective(m_camera->getZoomLevel(), m_width / m_height, 0.1f, 100.0f);

		// Level drawing
		m_currentScene->setViewMatrix(m_viewMatrix);
		m_currentScene->setProjectionMatrix(m_projectionMatrix);

		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Coordinate system matrices 
		glm::mat4 view = m_camera->GetViewMatrix();
		m_currentScene->setViewMatrix(view);

		Clock::time_point now = Clock::now();
		std::chrono::duration<double, std::ratio<1, 1000>> numberOfFrameSinceLast = now - m_lastTime;
		double numFrame = (numberOfFrameSinceLast.count() / (1000 / 24));
		if (numFrame > 1)
		{
			m_currentFrame += numFrame;
			m_lastTime = now;
		}

		m_currentScene->preDraw();
		m_currentScene->draw(m_currentFrame);
		m_currentScene->postDraw();
		
		// Swap the buffers
		glfwSwapBuffers(m_window);
	}
}

void Viewer::loadModel(const std::string& path, Vec3 position, std::string shaderProgram)
{
	m_currentScene->loadModel(path, position, shaderProgram);
}

void Viewer::removeSelectedObject()
{
	Object* objToBeRemoved = m_state->getLastSelectedObject();
	m_currentScene->removeObject(objToBeRemoved);
}

void Viewer::removeBackground(const Vec3& color)
{
	ModelContainer* lastSelectedObject = dynamic_cast<ModelContainer*>(m_state->getLastSelectedObject());
	
	if (lastSelectedObject)
		m_currentScene->addBackgroundToBeRemoved(lastSelectedObject, color);
}

void Viewer::moveCamera(double xoffset, double yoffset)
{
	m_camera->rotate(xoffset, yoffset);
}
