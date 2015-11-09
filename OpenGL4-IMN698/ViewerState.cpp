#include "ViewerState.h"
#include "Scene.h"
#include "Camera.h"
#include "Object.h"
#include "glm/glm/gtc/matrix_transform.hpp"


ViewerState::ViewerState() :
	m_viewingMode(ViewingMode::NORMAL),
	m_interactionMode(InteractionMode::NAVIGATION),
	m_transformationMode(TransformationMode::TRANSLATION)
{

}

void ViewerState::updateOperationData()
{
	
}

void ViewerState::startMouseOperation()
{

}

void ViewerState::handleMouseClick(GLFWwindow* window, int button, int action)
{
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		m_hasCurrentOperation = true;
		m_mousePosStartOp = glm::vec2(mouseX, mouseY);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		if (m_currentlySelectedObject)
			m_currentlySelectedObject->updateTransformationData();
		m_hasCurrentOperation = false;
	}

	//check if we select an object
	if (m_interactionMode == InteractionMode::TRANSFORMATION)
	{
		//create ray
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		float x = (2.0f * mouseX) / width - 1.0f;
		float y = 1.0f - (2.0f * mouseY) / height;
		float z = 1.0f;
		glm::vec3 ray(x,y,z);
		glm::vec4 ray_clip(ray.x, ray.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(Viewer::getInstance()->getProjectionMatrix()) * ray_clip;
		ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
		glm::vec4 ray_wor4 = (glm::inverse(Viewer::getInstance()->getViewMatrix()) * ray_eye);

		glm::vec3 ray_wor(ray_wor4.x, ray_wor4.y, ray_wor4.z);
		// don't forget to normalise the vector at some point
		ray_wor = glm::normalize(ray_wor);

		Ray r(Viewer::getInstance()->getCamera()->getPosition(),
				ray_wor, 
				std::numeric_limits<double>::lowest(), 
				std::numeric_limits<double>::max());

		//for each object in the scene, trace ray and check if it intersect the bounding box
		std::vector<Object*> objects;
		std::vector<Object*> sceneObjects = Viewer::getInstance()->getCurrentScene()->getObjects();
		std::vector<Object*> sceneLights = *(reinterpret_cast<std::vector<Object*>*>(&Viewer::getInstance()->getCurrentScene()->getLights()));
		objects.reserve(sceneObjects.size() + sceneLights.size());
		objects.insert(objects.end(), sceneObjects.begin(), sceneObjects.end());
		objects.insert(objects.end(), sceneLights.begin(), sceneLights.end()); 
		
		double minT = std::numeric_limits<double>::max();
		for (int i = 0; i < objects.size(); ++i)
		{
			double t0, t1;
			bool intersect = objects[i]->intersect(r, t0, t1);
			if (intersect && objects[i]->isVisible() && t0 < minT)
			{
				minT = t0;
				m_currentlySelectedObject = objects[i];
			}
		}
		if (minT == std::numeric_limits<double>::max())
			m_currentlySelectedObject = nullptr;
	}
}

void ViewerState::handleMouseMovement(double xPos, double yPos)
{
	if (m_hasCurrentOperation)
	{
		double xoffset = xPos - m_lastMousePosition.x;
		double yoffset = m_lastMousePosition.y - yPos; // Y coordinates range from bottom to top

		double xoffsetSinceOperationStart = xPos - m_mousePosStartOp.x;
		double yoffsetSinceOperationStart = yPos - m_mousePosStartOp.y;

		if (m_interactionMode == InteractionMode::NAVIGATION)
			Viewer::getInstance()->getCamera()->rotate(xoffset, yoffset);

		else if (m_interactionMode == InteractionMode::TRANSFORMATION && m_currentlySelectedObject)
		{
			//transform currently clicked object
			if (m_transformationMode == TransformationMode::ROTATION)
			{
				//in order to be precise, we compute a new matrix from the start each time
				//instead of adding small deltas to the matrix

				double rotationX = yoffsetSinceOperationStart / 360;
				double rotationY = xoffsetSinceOperationStart / 360;


				std::cout << rotationX << "  " << rotationY << std::endl;

				glm::mat4x4 m1;
				glm::mat4x4 m2;

				glm::vec4 v_x = glm::vec4(1, 0, 0, 0) * m_currentlySelectedObject->getModelMatrixStart();
				glm::vec4 v_y = glm::vec4(0, 1, 0, 0) * m_currentlySelectedObject->getModelMatrixStart();

				m1 = glm::rotate(m1, float(rotationX), Vec3(v_x.x, v_x.y, v_x.z));
				m2 = glm::rotate(m2, float(rotationY), Vec3(v_y.x, v_y.y, v_y.z));

				m_currentlySelectedObject->rotate(m1*m2);

			}
			else if (m_transformationMode == TransformationMode::TRANSLATION)
			{
				//we move on the camera plane 
				Matrix4x4 viewMat = Viewer::getInstance()->getCamera()->GetViewMatrix();

				glm::vec4 v(xoffset / 100, yoffset / 100, 0, 0);
				m_currentlySelectedObject->translate(Vec3(v*viewMat));
			}
		}
	}

	m_lastMousePosition.x = xPos;
	m_lastMousePosition.y = yPos;
}

void ViewerState::handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action)
{
	//-------------------------------------------------------------------------
	// Mode Independent
	//-------------------------------------------------------------------------

	// Escape application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//-------------------------------------------------------------------------
	// Viewing modes
	//-------------------------------------------------------------------------

	// Wireframe management
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		if (m_viewingMode == ViewingMode::NORMAL)
		{
			m_viewingMode = ViewingMode::WIREFRAME;
			Viewer::getInstance()->getCurrentScene()->drawAllLights(false);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else if (m_viewingMode == ViewingMode::WIREFRAME)
		{
			m_viewingMode = ViewingMode::LIGHT;
			Viewer::getInstance()->getCurrentScene()->drawAllLights(true);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else if (m_viewingMode == ViewingMode::LIGHT)
		{
			m_viewingMode = ViewingMode::NORMAL;
			Viewer::getInstance()->getCurrentScene()->drawAllLights(false);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

	}

	//-------------------------------------------------------------------------
	// Interaction and Transformation Modes
	//-------------------------------------------------------------------------

	else if (key == GLFW_KEY_T && action == GLFW_PRESS) //Transformation
	{
		m_interactionMode = TRANSFORMATION;
		m_transformationMode = TRANSLATION;
	}
	else if (key == GLFW_KEY_N && action == GLFW_PRESS) //Navigation
	{
		m_interactionMode = NAVIGATION;
	}
	else if (key == GLFW_KEY_L && action == GLFW_PRESS) //Locked
	{
		m_interactionMode = LOCKED;
	}
	else if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		m_transformationMode = ROTATION;
	}

	//-------------------------------------------------------------------------
	// Keep track of pressed keys 
	//-------------------------------------------------------------------------
	// Realtime (key held) interaction management
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			m_keys[key] = true;
		else if (action == GLFW_RELEASE)
			m_keys[key] = false;
	}
}


InteractionMode ViewerState::getInteractionMode()
{
	return m_interactionMode;
}