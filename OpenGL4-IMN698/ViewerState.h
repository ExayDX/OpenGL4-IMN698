
#ifndef __VIEWER_STATE_H__
#define __VIEWER_STATE_H__

#include "Viewer.h"

#include <gl/glew.h>
#include <GLFW/glfw3.h>


#include "Object.h"

#include "GLM\glm\glm.hpp"

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
	LIGHT //We can view light and move them
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
	ViewerState(Viewer* viewer);

	void handleMouseMovement(double xPos, double yPos);
	void handleMouseClick(GLFWwindow* window, int button, int action);
	void handleKeyboardInput(GLFWwindow* window, int key, int scancode, int action);

	InteractionMode getInteractionMode();


	bool m_keys[1024]; //keyboard keys
private:
	void startMouseOperation();
	void updateOperationData();

private:
	//Viewer modes
	InteractionMode m_interactionMode;
	ViewingMode m_viewingMode;
	TransformationMode m_transformationMode;


	ViewerOperation* m_currentOperation;
	std::stack<ViewerOperation*> operationStack;

	bool m_hasCurrentOperation;
	bool m_mouseIsClicked;
	bool m_firstClick;
	glm::vec2 m_lastMousePosition;
	glm::vec2 m_mousePosStartOp;
	Object* m_currentlySelectedObject;
	Viewer* m_viewer;
};

#endif //__VIEWER_STATE_H__