

#include "Animable.h"


Animation::Animation() :
	m_frameNumber(),
	m_loopback(false),
	m_frames()
{
}

Animation::Animation(std::list<Frame> frames) :
	m_frameNumber(frames.size()),
	m_loopback(false),
	m_frames(frames)
{
}


//insert frame at index
void Animation::addFrame(int frameNumber, Frame frame)
{
	const unsigned nodeToInsertBefore = frameNumber;
	std::list<Frame>::iterator it = m_frames.begin();

	for (unsigned node = 0; node < nodeToInsertBefore; ++node)
		++it;

	m_frames.insert(it, frame);
	m_frameNumber++;
}

Frame Animation::getFrame(int index) const
{
	if (m_loopback)
		index = index % (m_frameNumber - 1);

	if (index > m_frameNumber-1)
		return Frame();

	std::list<Frame>::const_iterator it = m_frames.begin();

	for (unsigned node = 0; node < index; ++node)
		++it;

	return *it;
}

void Animation::setLoopBack(bool val)
{
	m_loopback = val;
}