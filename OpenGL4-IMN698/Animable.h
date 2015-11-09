
#ifndef __ANIMABLE_H__
#define __ANIMABLE_H__

#include "Types.h"

#include <list>


struct Frame
{
	Frame() : m_transformation() {}
	Frame(Matrix4x4 m) : m_transformation(m) {}
	Matrix4x4 m_transformation;
};

class Animation
{
public:
	Animation();
	Animation(std::list<Frame> frames);
	
	Frame getFrame(int index) const;
	void addFrame(int index, Frame frame);
	void setLoopBack(bool val);

protected:
	int m_frameNumber;
	bool m_loopback;

	std::list<Frame> m_frames;
};

#endif //__ANIMABLE_H__