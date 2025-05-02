/**
 * @file logger.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Logger.h"
#include <chrono>
#include <filesystem>
#include <iostream>

Logger* Logger::_instance = nullptr;
std::mutex Logger::_mutex;

Logger::Logger() : minLevel(LogLevel::INFO) {
    // Tạo thư mục logs nếu chưa tồn tại
    std::filesystem::create_directory("logs");
    
    // Tạo tên file log với timestamp
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    std::ostringstream filename;
    filename << "logs/airlines_" 
             << std::put_time(&tm, "%Y%m%d_%H%M%S") 
             << ".log";
    
    // Mở file log
    _logFile.open(filename.str(), std::ios::out | std::ios::app);
    
    if (!_logFile.is_open()) {
        std::cerr << "Failed to open log file: " << filename.str() << "\n";
    }
    
    // Ghi thông tin khởi tạo
    log(LogLevel::INFO, "Logger initialized");
}

Logger::~Logger() {
    if (_logFile.is_open()) {
        log(LogLevel::INFO, "Logger shutting down");
        _logFile.close();
    }
}

Logger* Logger::getInstance() {
    std::lock_guard<std::mutex> lock(_mutex);
    if (_instance == nullptr) {
        _instance = new Logger();
    }
    return _instance;
}

std::string Logger::_getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    return oss.str();
}

std::string Logger::_getLevelString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        case LogLevel::FATAL:   return "FATAL";
        default:                return "UNKNOWN";
    }
}

void Logger::setMinLevel(LogLevel level) {
    std::lock_guard<std::mutex> lock(_mutex);
    minLevel = level;
    log(LogLevel::INFO, "Log level set to " + _getLevelString(level));
}

void Logger::log(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(_mutex);
    
    if (level < minLevel) {
        return;
    }
    
    std::string timestamp = _getTimestamp();
    std::string levelStr = _getLevelString(level);
    
    // Định dạng: [TIMESTAMP] [LEVEL] MESSAGE
    std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;
    
    // Ghi vào file
    if (_logFile.is_open()) {
        _logFile << logMessage << "\n";
        _logFile.flush();
    }
    
    // Ghi ra console
    // std::cout << logMessage << "\n";
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::fatal(const std::string& message) {
    log(LogLevel::FATAL, message);
}