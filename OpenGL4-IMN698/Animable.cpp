

#include "Animable.h"


Animation::Animation(int frameNumber) :
	m_frameNumber(frameNumber),
	m_loopback(false)
{
	for (int i = 0; i < frameNumber; ++i)
	{
		//Prepare the list of n frames but they are identity transformations
		m_frames.push_back(Frame(Matrix4x4())); 
	}
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