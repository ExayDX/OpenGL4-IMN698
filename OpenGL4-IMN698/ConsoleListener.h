
#ifndef __CONSOLE_LISTENER_H__
#define __CONSOLE_LISTENER_H__

#include "Types.h"
#include <thread>


class Viewer;

//listen for user input via console
class ConsoleListener
{
public:
	ConsoleListener(Viewer* viewer);
	~ConsoleListener();
	ConsoleListener(const ConsoleListener& listener) =delete;

	void listen();
	void stop();
	void sendCommand();

private:
	void analyseInput(const std::string& s);

	static bool pathIsValid(const std::string& path);
	static Vec3 positionStrToVec3(std::string& positionStr);
private:
	std::thread m_thread;
	Viewer* m_viewer;
};
#endif //__CONSOLE_LISTENER_H__