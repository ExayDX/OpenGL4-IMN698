#ifndef __SEPARABLE_SSS_H_
#define  __SEPARABLE_SSS_H_

#include "glm/glm/common.hpp"
#include "GL/glew.h"
#include "Quad.h"

#include <vector>

class SeparableSSSS
{
public:
	/*
	* strength is in the alpha channel of the color buffer,
	* ...
	*/
	SeparableSSSS(GLuint nSamples, GLfloat fovy, GLfloat sssWidth, bool followShape);

	// Getters/Setters
	void setWidth(float sssWidth) { m_sssWidth = sssWidth; }
	void setStrength(glm::vec3 strength){ m_strength = strength; }
	void setFalloff(glm::vec3 falloff){ m_falloff = falloff; }
	void updateFovy(GLfloat fovy){ m_fovy = fovy; }

	float getWidth() const { return m_sssWidth; }
	glm::vec3 getStrength() const { return m_strength; }
	glm::vec3 getFalloff() const{ return m_falloff; }
	std::string getKernelCode() const;

private : 
	std::vector<glm::vec4> m_kernel;
	GLuint m_nSamples;
	glm::vec3 m_strength; 
	glm::vec3 m_falloff;
	GLfloat m_sssWidth; 
	GLuint m_colorRenderBuffer; // RenderTarget
	Quad* m_quad; // TODO : Rly? What for? 
	GLfloat m_fovy; 

	//Uniforms : 
	/*
	ID3D10EffectScalarVariable *idVariable, *sssWidthVariable;
	ID3D10EffectVectorVariable *kernelVariable;
	ID3D10EffectShaderResourceVariable *colorTexVariable, *depthTexVariable, *strengthTexVariable;
	ID3D10EffectTechnique *technique;
	*/

	void calculateKernel(); 
	glm::vec3 profile(float r); 
	glm::vec3 gaussian(float variance, float r);


};


#endif // !__SEPARABLE_SSS_H_