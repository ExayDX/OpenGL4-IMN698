#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include "GL/glew.h"
#include <vector>
#include <map>

class FrameBuffer
{
public: 
	// Enums 
	enum class BufferType
	{
		eColor,
		eDepth,
		eStencil
	};

	// Typedefs
	typedef std::map<GLenum, GLuint> ParameterMap;
	typedef std::vector<GLuint> RenderBufferVec;
	typedef std::map<BufferType, RenderBufferVec* > RenderBufferMap;

	// Ctors/Dtors
	FrameBuffer();
	~FrameBuffer();

	// Public mehtods
	void bind(); 
	void unBind(); 
	void addBuffer(BufferType bufferType, GLuint attachmentPos, ParameterMap* parameteri = nullptr);
	GLuint getId(){ return m_id; }
	const GLuint getRenderBuffer(BufferType bufferType, GLuint id); 
	static void setDimensions(GLuint width, GLuint height);

protected : 
	GLuint m_id; 
	RenderBufferMap m_renderbuffers; 

	static GLuint m_width;
	static GLuint m_height;
};

#endif // !_FRAME_BUFFER_H_