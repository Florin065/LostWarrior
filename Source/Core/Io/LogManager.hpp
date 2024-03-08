#pragma once

#include <cstdlib>
#include <iostream>
#include <utility>


enum class LogLevel
{
    CRITICAL = 0,
    NORMAL,
    DEBUG,
};

class LogManager
{
public:
    LogManager(LogLevel logLevel): mLogLevel(logLevel) {}

    template <typename... Args>
    void Debug(Args&&... args)
    {
        Print(std::cout, "DEBUG", LogLevel::DEBUG, args...);
    }

    template <typename... Args>
    void Info(Args&&... args)
    {
        Print(std::cout, "INFO", LogLevel::NORMAL, args...);
    }

    template <typename... Args>
    void Error(Args&&... args)
    {
        Print(std::cerr, "ERROR", LogLevel::CRITICAL, args...);
    }

    template<typename T, typename... Args>
    void Assert(T condition, Args&&... args)
    {
        if (!condition)
        {
            Error(args...);
            exit(EXIT_FAILURE);
        }
    }

private:
    LogLevel mLogLevel;

    template <typename Arg, typename... Args>
    void Print(std::ostream& os,
               std::string const& type,
               LogLevel logLevel,
               Arg&& arg,
               Args&&... args)
    {
        if (logLevel <= mLogLevel)
        {
            os << "[" << type << "]\t" << std::forward<Arg>(arg);
            ((os << std::forward<Args>(args)), ...);
            os << std::endl;
        }
    }
};

