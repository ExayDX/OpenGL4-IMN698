#include "SeparableSSS.h"
#include "ShaderMacro.h"

#include <sstream>

/*This file is a translation from DirectX to OpenGL of the work of Jorge Jiminez and al. */ 

SeparableSSSS::SeparableSSSS(GLuint nSamples, GLfloat fovy, GLfloat sssWidth, bool followShape)
	: m_nSamples(nSamples)
	, m_sssWidth(sssWidth)
	, m_fovy(fovy)
{
	std::vector<ShaderMacro> defines; 
	std::stringstream s; 

	s << m_fovy; 
	std::string fovyText = s.str();
	ShaderMacro fovyMacro = { "SSSS_FOVY", fovyText };

	// TO COMPLETE
}

void SeparableSSSS::calculateKernel()
{
	const float range = m_nSamples > 20 ? 3.0f : 2.0f; 
	const float exponent = 2.0f; 

	m_kernel.resize(m_nSamples); 

	// Calculate the offsets
	float step = 2.0f * range / (m_nSamples - 1); 
	for (int i = 0; i < m_nSamples; ++i)
	{
		float o = -range + float(i) * step; 
		float sign = o < 0.0f ? -1.0f : 1.0f; 
		m_kernel[i].w = range * sign * abs(pow(o, exponent)) / pow(range, exponent); 
	}

	// Calculate the weights : 
	for (int i = 0; i < m_nSamples; ++i)
	{
		float w0 = i > 0 ? abs(m_kernel[i].w - m_kernel[i - 1].w) : 0.0f; 
		float w1 = i < m_nSamples - 1 ? abs(m_kernel[i].w - m_kernel[i + 1].w) : 0.0f; 
		float area = (w0 + w1) / 2.0f; 
		glm::vec3 t = area * profile(m_kernel[i].w);
		m_kernel[i].x = t.x; 
		m_kernel[i].y = t.y;
		m_kernel[i].z = t.z;
	}

	// We want the offset 0.0 to come first : 
	glm::vec4 t = m_kernel[m_nSamples / 2]; 
	for (int i = m_nSamples / 2; i > 0; --i)
		m_kernel[i] = m_kernel[i - 1];
	m_kernel[0] = t; 

	// Tweak them using the desired strength. The first one is : 
	//		lerp (1.0, m_kernel[0].rgb, strength)
	m_kernel[0].x = (1.0f - m_strength.x) * 1.0f + m_strength.x * m_kernel[0].x;
	m_kernel[0].y = (1.0f - m_strength.y) * 1.0f + m_strength.y * m_kernel[0].y;
	m_kernel[0].z = (1.0f - m_strength.z) * 1.0f + m_strength.z * m_kernel[0].z;

	// The others : 
	//		lerp(0.0, m_kernel[0].rgb, strength)
	for (int i = 1; i < m_nSamples; ++i)
	{
		m_kernel[i].x *= m_strength.x; 
		m_kernel[i].y *= m_strength.y;
		m_kernel[i].z *= m_strength.z;
	}

	// Finally, set 'em! 
	// TODO : send to shader

}

glm::vec3 SeparableSSSS::profile(float r)
{
	/**
	* We used the red channel of the original skin profile defined in
	* [d'Eon07] for all three channels. We noticed it can be used for green
	* and blue channels (scaled using the falloff parameter) without
	* introducing noticeable differences and allowing for total control over
	* the profile. For example, it allows to create blue SSS gradients, which
	* could be useful in case of rendering blue creatures.
	*/
	return  // 0.233f * gaussian(0.0064f, r) + /* We consider this one to be directly bounced light, accounted by the strength parameter (see @STRENGTH) */
		0.100f * gaussian(0.0484f, r) +
		0.118f * gaussian(0.187f, r) +
		0.113f * gaussian(0.567f, r) +
		0.358f * gaussian(1.99f, r) +
		0.078f * gaussian(7.41f, r);
}

glm::vec3 SeparableSSSS::gaussian(float variance, float r)
{
	glm::vec3 g; 
	for (int i = 0; i < 3; ++i)
	{
		float rr = r / (0.001f + m_falloff[i]);
		g[i] = exp((-(rr * rr)) / (2.0f * variance)) / (2.0f * 3.14f * variance); 
	}
	return g; 

}