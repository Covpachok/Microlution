//
// Created by heylc on 14.01.2023.
//

#ifndef MICROLUTION_LOGGER_HPP
#define MICROLUTION_LOGGER_HPP

#include <iostream>

#define DEBUG_LOG_INFO(msg) LogInfo(msg, __func__, __LINE__)
#define DEBUG_LOG_WARNING(msg) LogWarning(msg, __func__, __LINE__)
#define DEBUG_LOG_ERROR(msg) LogError(msg, __func__, __LINE__)

inline static void LogInfo(const std::string &msg, const std::string &funcName,
                           const std::size_t lineNumber)
{
    std::cout << "INFO::" << funcName << "::" << lineNumber << ": " << msg << std::endl << std::flush;
}

inline static void LogWarning(const std::string &msg, const std::string &funcName,
                              const std::size_t lineNumber)
{
    std::cout << "!WARNING::" << funcName << "::" << lineNumber << ": " << msg << std::endl << std::flush;
}

inline static void LogError(const std::string &msg, const std::string &funcName,
                            const std::size_t lineNumber)
{
    std::cout << "#ERROR::" << funcName << "::" << lineNumber << ": " << msg << std::endl << std::flush;
}

#endif //MICROLUTION_LOGGER_HPP
