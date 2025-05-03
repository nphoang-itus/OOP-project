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

<<<<<<< HEAD
 #include "Logger.h"
 #include <chrono>
 #include <iostream>
 #include <sstream>
 
 // Thêm include cần thiết cho xử lý ngoại lệ
 #include <stdexcept>
 
 // Thay đổi cách include filesystem tùy thuộc vào compiler
 #if defined(__has_include) && __has_include(<filesystem>)
 #include <filesystem>
 namespace fs = std::filesystem;
 #elif defined(__has_include) && __has_include(<experimental/filesystem>)
 #include <experimental/filesystem>
 namespace fs = std::experimental::filesystem;
 #else
 namespace fs {
     inline bool create_directory(const std::string&) { return true; }
 }
 #endif
 
 Logger* Logger::_instance = nullptr;
 std::mutex Logger::_mutex;
 
 Logger::Logger() : minLevel(LogLevel::INFO) {
     try {
         // Tạo thư mục logs nếu chưa tồn tại
         try {
             if (!fs::exists("logs")) {
                 if (!fs::create_directory("logs")) {
                     std::cerr << "Failed to create logs directory\n";
                 }
             }
         } catch (const std::exception& e) {
             std::cerr << "Error creating logs directory: " << e.what() << "\n";
         }
         
         // Tạo tên file log với timestamp
         auto now = std::chrono::system_clock::now();
         auto time = std::chrono::system_clock::to_time_t(now);
         
         // Sử dụng localtime_s (Windows) hoặc localtime_r (POSIX) thay vì localtime
         std::tm tm_buf;
         #if defined(_WIN32) || defined(_WIN64)
             localtime_s(&tm_buf, &time);
         #else
             localtime_r(&time, &tm_buf);
         #endif
         
         std::ostringstream filename;
         filename << "logs/airlines_";
         // Tạo định dạng thời gian thủ công để tránh lỗi
         char timebuf[64];
         std::strftime(timebuf, sizeof(timebuf), "%Y%m%d_%H%M%S", &tm_buf);
         filename << timebuf << ".log";
         
         // Mở file log
         _logFile.open(filename.str(), std::ios::out | std::ios::app);
         
         if (!_logFile.is_open()) {
             std::cerr << "Failed to open log file: " << filename.str() << "\n";
         } else {
             // Chỉ viết thông báo khởi tạo nếu file được mở thành công
             // Tránh gọi log() từ constructor để tránh deadlock
             std::string timestamp = _getTimestamp();
             _logFile << "[" << timestamp << "] [INFO] Logger initialized\n";
             _logFile.flush();
             
             // In ra console (để debugging)
             std::cout << "[" << timestamp << "] [INFO] Logger initialized\n";
         }
     } catch (const std::exception& e) {
         std::cerr << "Logger initialization error: " << e.what() << "\n";
     }
 }
 
 Logger::~Logger() {
     try {
         if (_logFile.is_open()) {
             // Tránh gọi log() từ destructor
             std::string timestamp = _getTimestamp();
             _logFile << "[" << timestamp << "] [INFO] Logger shutting down\n";
             _logFile.flush();
             _logFile.close();
         }
     } catch (const std::exception& e) {
         std::cerr << "Logger shutdown error: " << e.what() << "\n";
     }
 }
 
 Logger* Logger::getInstance() {
     // Double-checked locking để tránh overhead của mutex
     if (_instance == nullptr) {
         std::lock_guard<std::mutex> lock(_mutex);
         if (_instance == nullptr) {
             try {
                 _instance = new Logger();
             } catch (const std::exception& e) {
                 std::cerr << "Failed to create Logger instance: " << e.what() << "\n";
                 throw;
             }
         }
     }
     return _instance;
 }
 
 std::string Logger::_getTimestamp() {
     try {
         auto now = std::chrono::system_clock::now();
         auto time = std::chrono::system_clock::to_time_t(now);
         
         // Sử dụng localtime_s (Windows) hoặc localtime_r (POSIX) thay vì localtime
         std::tm tm_buf;
         #if defined(_WIN32) || defined(_WIN64)
             localtime_s(&tm_buf, &time);
         #else
             localtime_r(&time, &tm_buf);
         #endif
         
         char timebuf[64];
         std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_buf);
         return std::string(timebuf);
     } catch (const std::exception& e) {
         std::cerr << "Error getting timestamp: " << e.what() << "\n";
         return "Unknown time";
     }
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
     try {
         std::lock_guard<std::mutex> lock(_mutex);
         minLevel = level;
         
         // Tránh gọi log() từ setMinLevel vì nó cũng sử dụng lock
         if (_logFile.is_open()) {
             std::string timestamp = _getTimestamp();
             std::string levelStr = _getLevelString(level);
             _logFile << "[" << timestamp << "] [INFO] Log level set to " << levelStr << "\n";
             _logFile.flush();
             
             // In ra console (để debugging)
             std::cout << "[" << timestamp << "] [INFO] Log level set to " << levelStr << "\n";
         }
     } catch (const std::exception& e) {
         std::cerr << "Error setting log level: " << e.what() << "\n";
     }
 }
 
 void Logger::log(LogLevel level, const std::string& message) {
     try {
         // Nếu cấp độ log thấp hơn cấp độ tối thiểu, bỏ qua
         if (level < minLevel) {
             return;
         }
         
         std::lock_guard<std::mutex> lock(_mutex);
         
         std::string timestamp = _getTimestamp();
         std::string levelStr = _getLevelString(level);
         
         // Định dạng: [TIMESTAMP] [LEVEL] MESSAGE
         std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;
         
         // Ghi vào file
         if (_logFile.is_open()) {
             _logFile << logMessage << "\n";
             _logFile.flush();
         } else {
             // Nếu file không mở được, in ra console
             std::cerr << "Log file not open, writing to console: " << logMessage << "\n";
         }
         
         // Ghi ra console cho các cấp độ ERROR và FATAL
         if (level >= LogLevel::ERROR) {
             std::cerr << logMessage << "\n";
         }
     } catch (const std::exception& e) {
         // Sử dụng cerr để tránh đệ quy vô hạn
         std::cerr << "Error logging message: " << e.what() << "\n";
     }
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
=======
#include "Logger.h"
#include <chrono>
#include <iostream>
#include <sstream>

// Thêm include cần thiết cho xử lý ngoại lệ
#include <stdexcept>

// Thay đổi cách include filesystem tùy thuộc vào compiler
#if defined(__has_include) && __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined(__has_include) && __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
namespace fs {
    inline bool create_directory(const std::string&) { return true; }
}
#endif

Logger* Logger::_instance = nullptr;
std::mutex Logger::_mutex;

Logger::Logger() : minLevel(LogLevel::INFO) {
    try {
        // Tạo thư mục logs nếu chưa tồn tại
        try {
            if (!fs::exists("logs")) {
                if (!fs::create_directory("logs")) {
                    std::cerr << "Failed to create logs directory\n";
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Error creating logs directory: " << e.what() << "\n";
        }
        
        // Tạo tên file log với timestamp
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        // Sử dụng localtime_s (Windows) hoặc localtime_r (POSIX) thay vì localtime
        std::tm tm_buf;
        #if defined(_WIN32) || defined(_WIN64)
            localtime_s(&tm_buf, &time);
        #else
            localtime_r(&time, &tm_buf);
        #endif
        
        std::ostringstream filename;
        filename << "logs/airlines_";
        // Tạo định dạng thời gian thủ công để tránh lỗi
        char timebuf[64];
        std::strftime(timebuf, sizeof(timebuf), "%Y%m%d_%H%M%S", &tm_buf);
        filename << timebuf << ".log";
        
        // Mở file log
        _logFile.open(filename.str(), std::ios::out | std::ios::app);
        
        if (!_logFile.is_open()) {
            std::cerr << "Failed to open log file: " << filename.str() << "\n";
        } else {
            // Chỉ viết thông báo khởi tạo nếu file được mở thành công
            // Tránh gọi log() từ constructor để tránh deadlock
            std::string timestamp = _getTimestamp();
            _logFile << "[" << timestamp << "] [INFO] Logger initialized\n";
            _logFile.flush();
            
            // In ra console (để debugging)
            std::cout << "[" << timestamp << "] [INFO] Logger initialized\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Logger initialization error: " << e.what() << "\n";
    }
}

Logger::~Logger() {
    try {
        if (_logFile.is_open()) {
            // Tránh gọi log() từ destructor
            std::string timestamp = _getTimestamp();
            _logFile << "[" << timestamp << "] [INFO] Logger shutting down\n";
            _logFile.flush();
            _logFile.close();
        }
    } catch (const std::exception& e) {
        std::cerr << "Logger shutdown error: " << e.what() << "\n";
    }
}

Logger* Logger::getInstance() {
    // Double-checked locking để tránh overhead của mutex
    if (_instance == nullptr) {
        std::lock_guard<std::mutex> lock(_mutex);
        if (_instance == nullptr) {
            try {
                _instance = new Logger();
            } catch (const std::exception& e) {
                std::cerr << "Failed to create Logger instance: " << e.what() << "\n";
                throw;
            }
        }
    }
    return _instance;
}

std::string Logger::_getTimestamp() {
    try {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        // Sử dụng localtime_s (Windows) hoặc localtime_r (POSIX) thay vì localtime
        std::tm tm_buf;
        #if defined(_WIN32) || defined(_WIN64)
            localtime_s(&tm_buf, &time);
        #else
            localtime_r(&time, &tm_buf);
        #endif
        
        char timebuf[64];
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", &tm_buf);
        return std::string(timebuf);
    } catch (const std::exception& e) {
        std::cerr << "Error getting timestamp: " << e.what() << "\n";
        return "Unknown time";
    }
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
    try {
        std::lock_guard<std::mutex> lock(_mutex);
        minLevel = level;
        
        // Tránh gọi log() từ setMinLevel vì nó cũng sử dụng lock
        if (_logFile.is_open()) {
            std::string timestamp = _getTimestamp();
            std::string levelStr = _getLevelString(level);
            _logFile << "[" << timestamp << "] [INFO] Log level set to " << levelStr << "\n";
            _logFile.flush();
            
            // In ra console (để debugging)
            std::cout << "[" << timestamp << "] [INFO] Log level set to " << levelStr << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error setting log level: " << e.what() << "\n";
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    try {
        // Nếu cấp độ log thấp hơn cấp độ tối thiểu, bỏ qua
        if (level < minLevel) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(_mutex);
        
        std::string timestamp = _getTimestamp();
        std::string levelStr = _getLevelString(level);
        
        // Định dạng: [TIMESTAMP] [LEVEL] MESSAGE
        std::string logMessage = "[" + timestamp + "] [" + levelStr + "] " + message;
        
        // Ghi vào file
        if (_logFile.is_open()) {
            _logFile << logMessage << "\n";
            _logFile.flush();
        } else {
            // Nếu file không mở được, in ra console
            std::cerr << "Log file not open, writing to console: " << logMessage << "\n";
        }
        
        // Ghi ra console cho các cấp độ ERROR và FATAL
        if (level >= LogLevel::ERROR) {
            std::cerr << logMessage << "\n";
        }
    } catch (const std::exception& e) {
        // Sử dụng cerr để tránh đệ quy vô hạn
        std::cerr << "Error logging message: " << e.what() << "\n";
    }
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
>>>>>>> develop
