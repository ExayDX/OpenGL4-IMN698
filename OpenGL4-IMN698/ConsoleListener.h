
#ifndef __CONSOLE_LISTENER_H__
#define __CONSOLE_LISTENER_H__

#include "Types.h"
#include <thread>


//listen for user input via console
class ConsoleListener
{
public:
	ConsoleListener();
	~ConsoleListener();
	ConsoleListener(const ConsoleListener& listener) =delete;

	void listen();

private:
	void analyseInput(const std::string& s);
	static bool pathIsValid(const std::string& path);
	static bool shaderProgramIsValid(const std::string& shaderProgram);
	static Vec3 positionStrToVec3(std::string& positionStr);

private:
	std::thread m_thread;
};
#endif //__CONSOLE_LISTENER_H__