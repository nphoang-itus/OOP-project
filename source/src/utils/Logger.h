/**
 * @file Logger.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <memory>
#include <fstream>
#include <mutex>
#include <vector>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class ILogHandler {
public:
    virtual ~ILogHandler() = default;
    virtual void write(LogLevel level, const std::string& timestamp, const std::string& message) = 0;
};

class ConsoleLogHandler : public ILogHandler {
public:
    void write(LogLevel level, const std::string& timestamp, const std::string& message) override;
};

class FileLogHandler : public ILogHandler {
private:
    std::ofstream _logFile;
    std::mutex _fileMutex;

public:
    explicit FileLogHandler(const std::string& fileName);
    ~FileLogHandler() override;
    void write(LogLevel level, const std::string& timestamp, const std::string& message) override;
};

class Logger {
private:
    static std::shared_ptr<Logger> _instance;
    static std::mutex _instanceMutex;

    std::vector<std::shared_ptr<ILogHandler>> _handlers;
    std::mutex _handlersMutex;
    LogLevel _minLevel;

    Logger();
    std::string _getTimestamp() const;
    std::string _getLevelString(LogLevel level) const;

public:
    static std::shared_ptr<Logger> getInstance();
    
    // Xoá bỏ các constructor
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    ~Logger();
    void setMinLevel(LogLevel level);
    void addHandler(std::shared_ptr<ILogHandler> handler);
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);
};

#endif