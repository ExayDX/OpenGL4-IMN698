

#include "Animable.h"


Animation::Animation() :
	m_loopback(false),
	m_frames()
{
}

Animation::Animation(std::map<int, Frame> frames) :
	m_loopback(false),
	m_frames(frames)
{
}


//insert frame at index
void Animation::addFrame(int frameNumber, Frame frame)
{
	m_frames[frameNumber] = frame;
}

Frame Animation::getFrame(int index) const
{
	if (m_loopback)
		index = index % (m_frames.size() - 1);

	auto it = m_frames.find(index);
	if (it != m_frames.end())
		return it->second;

	return Frame();
}

void Animation::setLoopBack(bool val)
{
	m_loopback = val;
}