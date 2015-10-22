#ifndef _RENDER_BUFFER_H_
#define _RENDER_BUFFER_H_

#include "GL/glew.h"
#include <map>

class RenderBuffer
{
public : 

	typedef std::map<GLenum, GLuint> ParameterMap;
	
	enum class BufferType
	{
		eColor,
		eDepth,
		eStencil
	};

	RenderBuffer(BufferType aType, ParameterMap* parameteri = nullptr);

	static void setDimensions(GLuint width, GLuint height);
	virtual GLuint getData() { return m_data; }; 

	static GLuint m_width;
	static GLuint m_height;

protected:
	static GLuint m_nColorBuffer;
	static GLuint m_nDepthBuffer;
	static GLuint m_nStencilBuffer;

	BufferType m_type; 
	GLuint m_id; 
	GLuint m_data; 

};


#endif // _RENDER_BUFFER_H_ 