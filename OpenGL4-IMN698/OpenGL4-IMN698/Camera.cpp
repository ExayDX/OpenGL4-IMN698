#include "Camera.h"
#include "glm/glm/gtc/matrix_transform.hpp"

Camera::Camera(glm::vec3* upVector, glm::vec3* initialPosition, glm::vec3* initialTarget)
	: m_yaw(-90.0f)
	, m_pitch(0.0f)
	, m_mvtSpeed(3.0f)
	, m_rotSpeed(0.25f)
	, m_zoom(45.0f)
{
	if (upVector)
		m_worldUp = *upVector;
	else
		m_worldUp = glm::vec3(0, 1, 0);

	if (initialPosition)
		m_position = *initialPosition;
	else
		m_position = glm::vec3(0, 0, 0); 

	if (initialTarget)
		m_target = *initialTarget;
	else
		m_target = glm::vec3(0, 0, 0);

	m_zNeg = glm::normalize(m_position - m_target);
	m_xPos = glm::normalize(glm::cross(m_worldUp, m_zNeg));
	m_yPos = glm::cross(m_zNeg, m_xPos);

	updateVectors(); 
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_position + m_zNeg, m_yPos); 
}

void Camera::move(CameraDirection direction, GLfloat deltaTime)
{
	GLfloat velocity = m_mvtSpeed * deltaTime; 

	if (direction == CameraDirection::eForward)
		m_position += m_zNeg * velocity; 
	if (direction == CameraDirection::eBackward)
		m_position -= m_zNeg * velocity; 
	if (direction == CameraDirection::eLeft)
		m_position -= m_xPos * velocity; 
	if (direction == CameraDirection::eRight)
		m_position += m_xPos * velocity; 
}

void Camera::rotate(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch /* = true */)
{
	xoffset *= m_rotSpeed; 
	yoffset *= m_rotSpeed; 

	m_yaw += xoffset;
	m_pitch += yoffset; 

	if (constrainPitch)
	{
		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;
	}

	updateVectors(); 
}

void Camera::zoom(GLfloat yoffset)
{
	if (m_zoom >= 1.0f && m_zoom <= 45.0f)
		m_zoom -= yoffset; 
	if (m_zoom <= 1.0f)
		m_zoom = 1.0f;
	if (m_zoom >= 45.0f)
		m_zoom = 45.0f; 
}

void Camera::updateVectors()
{
	// Calculate the new front vector
	glm::vec3 newZNeg;
	newZNeg.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	newZNeg.y = sin(glm::radians(m_pitch));
	newZNeg.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_zNeg = glm::normalize(newZNeg); 

	// Recalculate up and right vectors
	this->m_xPos = glm::normalize(glm::cross(this->m_zNeg, this->m_worldUp)); 
	this->m_yPos = glm::normalize(glm::cross(this->m_xPos, this->m_zNeg));
}