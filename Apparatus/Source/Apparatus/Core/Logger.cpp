#include "Logger.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

std::ofstream Logger::file;

void Logger::open(const std::string& folder)
{
	std::string path = folder + getTimeString("%d-%m-%y %H-%M-%S.log");

	if (file.is_open())
	{
		file.close();
	}

	file.open(path, std::fstream::out | std::fstream::app);
}

void Logger::close()
{
	if (file.is_open())
	{
		file.close();
	}
}

void Logger::log(const std::string& text, LogLevel level)
{
	if (!file.is_open())
	{
		return;
	}

	std::stringstream out;
	out << getTimeString("[%d/%m/%y][%H:%M:%S]");

	switch (level)
	{
	case Trace:
		out << "[TRACE]: ";
		break;
	case Info:
		out << "[INFO]: ";
		break;
	case Warning:
		out << "[WARNING]: ";
		break;
	case Error:
		out << "[ERROR]: ";
		break;
	default:
		break;
	}

	out << text << '\n';

	file << out.str();
	std::cout << out.str();
}

std::string Logger::getTimeString(const char* format)
{
	std::stringstream out;

	const time_t time = std::time(nullptr);
	if (const tm* const currentTime = std::localtime(&time))
	{
		out << std::put_time(currentTime, format);
	}

	return out.str();
}
