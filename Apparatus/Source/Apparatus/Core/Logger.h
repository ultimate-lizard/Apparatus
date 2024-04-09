#pragma once

#include <string>
#include <fstream>
#include <exception>

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
	Logger(const Logger&) = delete;
	Logger(Logger&&) = delete;

	static void open(const std::string& folder);
	static void close();

	static void log(const std::string& text, LogLevel level);

private:
	static std::string getTimeString(const char* format);

	static std::ofstream file;
};
