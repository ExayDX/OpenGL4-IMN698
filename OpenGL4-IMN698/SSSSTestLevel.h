#ifndef _SSSS_TEST_LEVEL_H_
#define _SSSS_TEST_LEVEL_H_

#include "Scene.h"
#include <gl/glew.h>
#include <GLFW/glfw3.h>

class SSSSTestLevel : public Scene
{
public:
	SSSSTestLevel();
	virtual void draw(int currentFrame) override;
	virtual void setFOVY(GLfloat fovy){ m_fovy = fovy; }

protected:
	GLfloat m_fovy;
	GLuint m_color;

	virtual void levelSetup() override;
	virtual void lightSetup() override;
	virtual void buffersSetup() override;
	virtual void createShaderPrograms() override;
	virtual void createMaterials() override;

}; 

#endif // _SSSS_TEST_LEVEL_H_
