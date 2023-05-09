#ifndef MICROLUTION_LOGGER_HPP
#define MICROLUTION_LOGGER_HPP

#include <iostream>
#include "raylib.h"
#include <cmath>

#define DEBUG_LOG_INFO(msg) LogInfo(msg, __func__, __LINE__)
#define DEBUG_LOG_WARNING(msg) LogWarning(msg, __func__, __LINE__)
#define DEBUG_LOG_ERROR(msg) LogError(msg, __func__, __LINE__)

inline static void LogInfo(const std::string &msg, const std::string &funcName,
                           const std::size_t lineNumber)
{
	std::cout << ceil(GetTime()) << " INFO::" << funcName << "::" << lineNumber << ": " << msg << std::endl;
}

inline static void LogWarning(const std::string &msg, const std::string &funcName,
                              const std::size_t lineNumber)
{
	std::cout << ceil(GetTime()) << " !WARNING::" << funcName << "::" << lineNumber << ": " << msg << std::endl;
}

inline static void LogError(const std::string &msg, const std::string &funcName,
                            const std::size_t lineNumber)
{
	std::cout << ceil(GetTime()) << " #ERROR::" << funcName << "::" << lineNumber << ": " << msg << std::endl;
}

#endif //MICROLUTION_LOGGER_HPP
