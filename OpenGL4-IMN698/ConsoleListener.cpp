

#include "ConsoleListener.h"
#include "Viewer.h"

#include <iostream>
#include <string>
#include <fstream>

ConsoleListener::ConsoleListener()
{
	m_thread = std::thread(&ConsoleListener::listen, this);
}

ConsoleListener::~ConsoleListener()
{
	m_thread.detach();
}

void ConsoleListener::listen()
{
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	std::cout << "Enter command: ";
	std::string command;
	std::cin >> command;
	analyseInput(command);
}

void ConsoleListener::analyseInput(const std::string& action)
{

	// load <path> <position>
	if (action == "help")
	{
		std::cout << "Available command: " << std::endl;
		std::cout << "1) load" << std::endl;
		std::cout << "2) list" << std::endl;
		std::cout << "3) remove" << std::endl;
	}
	else if (action == "list")
	{
		std::cout << "What do you want to list? : ";
		std::string arg;
		std::cin >> arg;

		if (arg == "shaders")
		{
			//TODO
			std::vector<std::string> shaderList = Viewer::getInstance()->getCurrentScene()->getShaderList();

			for each (std::string shader in shaderList)
			{
				std::cout << "    " << shader << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid argument" << std::endl;
		}
	}
	else if (action == "load")
	{
		std::cout << "Enter path to model: ";
		std::string path;
		std::cin >> path;

		if (pathIsValid(path))
		{

			std::cout << "Enter position (x,y,z): ";
			std::string positionStr;
			std::cin >> positionStr;
			Vec3 position;
			bool positionValid = false;
			try 
			{
				position = positionStrToVec3(positionStr);
				positionValid = true;
			}
			catch (const std::exception e)
			{
				std::cout << "Invalid position, aborting command" << std::endl;
			}

			if (positionValid)
			{
				std::cout << "Enter shading program: ";
				std::string shaderProgram;
				std::cin >> shaderProgram;

				if (shaderProgramIsValid(shaderProgram))
					Viewer::getInstance()->loadModel(path, position, shaderProgram);
				else
					std::cout << "Invalid shader program, aborting command" << std::endl;
			}
		}
		else
		{
			std::cout << "Invalid path, aborting command" << std::endl;
		}
	}
	else if (action == "remove")
	{
		std::cout << "What do you want to remove (background/object): ";
		std::string answer;
		std::cin >> answer;

		if (answer == "background")
		{
			std::cout << "Enter background color (r,g,b) [0, 255]: ";
			std::string colorStr;
			std::cin >> colorStr;

			Vec3 color;
			if (isValidColor(colorStr, color))
				Viewer::getInstance()->removeBackground(color);
		}
		else if (answer == "object")
		{
			Viewer::getInstance()->removeSelectedObject();
		}
		else
		{
			std::cout << "Invalid answer, aborting command" << std::endl;
		}
	}
	else
	{
		std::cout << "Invalid input" << std::endl;
	}

	listen();
}

bool ConsoleListener::pathIsValid(const std::string& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		return false;
	}
	return true;
}

bool ConsoleListener::isValidColor(std::string& colorStr, Vec3& color)
{
	size_t posFirstComma = colorStr.find_first_of(',');
	std::string x = colorStr.substr(1, posFirstComma);
	colorStr = colorStr.substr(posFirstComma + 1);

	size_t posSecondComma = colorStr.find_first_of(',');
	std::string y = colorStr.substr(0, posSecondComma);
	colorStr = colorStr.substr(posSecondComma + 1);

	std::string z = colorStr.substr(0, colorStr.size() - 1);

	try
	{
		color = Vec3(std::stod(x), std::stod(y), std::stod(z));
	}
	catch (std::exception e)
	{
		return false;
	}

	return color.r >= 0 && color.g >= 0 && color.b >= 0 && 
		color.r < 256 && color.g < 256 && color.b < 256;
}

bool ConsoleListener::shaderProgramIsValid(const std::string& shaderProgram)
{
	std::vector<std::string> shaderList =  Viewer::getInstance()->getCurrentScene()->getShaderList();

	for (int i = 0; i < shaderList.size(); ++i)
	{
		if (shaderList[i] == shaderProgram)
			return true;
	}

	return false;
}

Vec3 ConsoleListener::positionStrToVec3(std::string& positionStr)
{
	size_t posFirstComma = positionStr.find_first_of(',');
	std::string x = positionStr.substr(1, posFirstComma);
	positionStr = positionStr.substr(posFirstComma + 1);

	size_t posSecondComma = positionStr.find_first_of(',');
	std::string y = positionStr.substr(0, posSecondComma);
	positionStr = positionStr.substr(posSecondComma + 1);

	std::string z = positionStr.substr(0, positionStr.size()-1);

	return Vec3(std::stod(x), std::stod(y), std::stod(z));
}