#ifndef CAMERA_H
#define CAMERA_H

#include "GLFW/glfw3.h"
#include "GLM/glm/glm.hpp"

class Camera
{
public:

	enum CameraDirection
	{
		eForward = 0x01,
		eBackward = 0x02,
		eLeft = 0x04,
		eRight = 0x08
	};

	Camera(glm::vec3* upVector = nullptr, glm::vec3* initialPosition = nullptr, glm::vec3* initalTarget = nullptr);
	~Camera(){}; 

	void move(CameraDirection displacement, GLfloat deltaTime); 
	void rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void zoom(GLfloat yoffest); 
	glm::mat4 GetViewMatrix(); 

	GLfloat getZoomLevel()
	{
		return m_zoom; 
	}
protected:

	glm::vec3 m_position; 
	glm::vec3 m_target; 
	glm::vec3 m_worldUp; 

	// Cameras axis in world coordinates
	glm::vec3 m_zNeg; // Front
	glm::vec3 m_xPos; // Right
	glm::vec3 m_yPos; // Up

	GLfloat m_mvtSpeed; 
	GLfloat m_rotSpeed;
	GLfloat m_zoom; 

	GLfloat m_yaw;
	GLfloat m_pitch; 

	
	void updateVectors(); 

private:
};

#endif