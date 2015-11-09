
#ifndef __VIEWER_STATE_H__
#define __VIEWER_STATE_H__

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include "GLM\glm\glm.hpp"

#include "Viewer.h"
#include "Object.h"

#include <stack>


class Viewer;

enum InteractionMode 
{
	NAVIGATION,
	TRANSFORMATION,
	LOCKED
};

enum ViewingMode
{
	NORMAL,
	WIREFRAME,
	LIGHT //We can view lights and move them
};

enum TransformationMode
{
	ROTATION,
	TRANSLATION
};

struct ViewerOperation
{

};

class ViewerState
{
public:
	ViewerState();

	void handleMouseMovement(double xPos, double yPos);
	void handleMouseClick(GLFWwindow* window, int button, int action);
	void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action);

	InteractionMode getInteractionMode();
	bool* getKeys(){ return m_keys; }

private:
	void startMouseOperation();
	void updateOperationData();

private:
	//Viewer modes
	InteractionMode m_interactionMode;
	ViewingMode m_viewingMode;
	TransformationMode m_transformationMode;

	// Operations
	ViewerOperation* m_currentOperation;
	std::stack<ViewerOperation*> operationStack;

	// Interaction variables
	bool m_keys[1024]; // All the keys that can be pressed
	bool m_hasCurrentOperation;
	bool m_mouseIsClicked;
	bool m_firstClick;
	glm::vec2 m_lastMousePosition;
	glm::vec2 m_mousePosStartOp;
	Object* m_currentlySelectedObject;
};

#endif //__VIEWER_STATE_H__