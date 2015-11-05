#include "FrameBuffer.h"
#include <cassert>

GLuint FrameBuffer::m_width = 0;
GLuint FrameBuffer::m_height = 0;

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1, &m_id);
}

FrameBuffer::~FrameBuffer()
{
	for (auto it = m_renderbuffers.begin(); it != m_renderbuffers.end(); ++it)
	{
		delete it->second; it->second = nullptr;
	}
}

void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id); 
}

void FrameBuffer::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}


void FrameBuffer::addBuffer(BufferType bufferType, GLuint attachmentPos, ParameterMap* parameteri /* = nullptr */)
{
	GLuint data; 
	if (bufferType == BufferType::eColor)
	{
		// Create Texture (Color buffer)
		glGenTextures(1, &data);
		glBindTexture(GL_TEXTURE_2D, data);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);

		// Set Texture parameters
		if (parameteri)
		{
			for (auto it = parameteri->begin(); it != parameteri->end(); ++it)
			{
				glTexParameteri(GL_TEXTURE_2D, it->first, it->second);
			}
		}

		// Allocate buffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + attachmentPos, GL_TEXTURE_2D, data, 0);
	}

	else if (bufferType == BufferType::eDepth)
	{
		glGenRenderbuffers(1, &data);
		glBindRenderbuffer(GL_RENDERBUFFER, data);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, data);

	}
	else if (bufferType == BufferType::eStencil)
	{
		glGenRenderbuffers(1, &data);
		glBindRenderbuffer(GL_RENDERBUFFER, data);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, data);
	}
	else
	{
		std::string errorMessage = "ERROR::RENDERBUFFER TYPE IS NOT IMPLEMENTED\n";
		throw std::runtime_error(errorMessage);
	}
	// Create vector of bufferType if doesn't exist
	if (m_renderbuffers.find(bufferType) == m_renderbuffers.end())
	{
		m_renderbuffers[bufferType] = new RenderBufferVec; 
	}

	// add the buffer to the buffermap
	m_renderbuffers[bufferType]->push_back(data); 
}


void FrameBuffer::setDimensions(GLuint width, GLuint height)
{
	if (m_width == 0 && m_height == 0)
	{
		m_width = width;
		m_height = height;
	}
}

const GLuint FrameBuffer::getRenderBuffer(BufferType bufferType, GLuint id)
{
	return m_renderbuffers[bufferType]->at(id);
}