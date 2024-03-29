#pragma once

#include <string>
#include <fstream>

//#ifdef _DEBUG
//#define LOG(text, level) Logger::log(text, level)
//#else
//#define LOG(text, level)
//#endif

#define LOG(text, level) Logger::log(text, level)

enum LogLevel
{
	Trace,
	Info,
	Warning,
	Error
};

class Logger
{
public:
	static void open(const std::string& folder);
	static void close();

	static void log(const std::string& text, LogLevel level);

private:
	static std::string getTimeString(const char* format);

	static std::ofstream file;
};
