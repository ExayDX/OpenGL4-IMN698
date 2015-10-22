#include "RenderBuffer.h"

GLuint RenderBuffer::m_width = 0;
GLuint RenderBuffer::m_height = 0;
GLuint RenderBuffer::m_nColorBuffer = 0;
GLuint RenderBuffer::m_nStencilBuffer = 0;

RenderBuffer::RenderBuffer(BufferType aType, ParameterMap* parameteri)
	: m_type(aType)
{
	if (aType == BufferType::eColor)
	{
		// Create Texture (Color buffer)
		glGenTextures(1, &m_data);
		glBindTexture(GL_TEXTURE_2D, m_data); 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr); 
		
		// Set Texture parameters
		if (parameteri)
		{
			for (auto it = parameteri->begin(); it != parameteri->end(); ++it)
			{
				glTexParameteri(GL_TEXTURE_2D, it->first, it->second);
			}
		}

		// Get the number of the current colorBuffer based on how many has been created
		m_id = m_nColorBuffer;
		m_nColorBuffer++;

		// Allocate buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_id, GL_TEXTURE_2D, m_data, 0); 
	}

	else if (aType == BufferType::eDepth)
	{
		m_id = 0; 

		glGenRenderbuffers(1, &m_data);
		glBindRenderbuffer(GL_RENDERBUFFER, m_data);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_data); 

	}
	else if (aType == BufferType::eStencil)
	{
		//...
	}
	else
	{
		std::string errorMessage = "ERROR::RENDERBUFFER TYPE IS NOT IMPLEMENTED\n";
		throw std::runtime_error(errorMessage);
	}
}

void RenderBuffer::setDimensions(GLuint width, GLuint height)
{
	if (m_width == 0 && m_height == 0)
	{
		m_width = width;
		m_height = height;
	}
}