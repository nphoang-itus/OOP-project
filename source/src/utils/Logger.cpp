#include "Logger.h"

#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <memory>

#if defined(__has_include) && __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif defined(__has_include) && __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
namespace fs {
    inline bool exists(const std::string&) { return true; }
    inline bool create_directory(const std::string&) { return true; }
}
#endif

std::shared_ptr<Logger> Logger::_instance = nullptr;
std::mutex Logger::_instanceMutex;

void ConsoleLogHandler::write(LogLevel level, const std::string& timestamp, const std::string& message) {
    std::string levelStr = "";

    switch (level) {
        case LogLevel::DEBUG:   levelStr = "DEBUG";     break;
        case LogLevel::INFO:    levelStr = "INFO";      break;
        case LogLevel::WARNING: levelStr = "WARNING";   break;
        case LogLevel::ERROR:   levelStr = "ERROR";     break;
        case LogLevel::FATAL:   levelStr = "FATAL";     break;
        default:                levelStr = "UNKNOWN";   break;
    }
    
    // Sử dụng màu sắc cho các mức độ log khác nhau
    std::string colorCode = "";
    if (level == LogLevel::DEBUG) {
        colorCode = "\033[37m"; // White
    } else if (level == LogLevel::INFO) {
        colorCode = "\033[32m"; // Green
    } else if (level == LogLevel::WARNING) {
        colorCode = "\033[33m"; // Yellow
    } else if (level == LogLevel::ERROR) {
        colorCode = "\033[31m"; // Red
    } else if (level == LogLevel::FATAL) {
        colorCode = "\033[35m"; // Magenta
    }
    
    std::string resetCode = "\033[0m";
    
    // Ghi ra console
    std::ostream& out = (level >= LogLevel::ERROR) ? std::cerr : std::cout;
    out << "[" << timestamp << "] " << colorCode << "[" << levelStr << "]" << resetCode << " " << message << std::endl;
}

FileLogHandler::FileLogHandler(const std::string& filename) {
    try {
        // Tạo thư mục logs nếu chưa tồn tại
        if (!fs::exists("logs")) {
            if (!fs::create_directory("logs")) {
                throw std::runtime_error("Failed to create logs directory");
            }
        }
        
        // Mở file log
        _logFile.open(filename, std::ios::out | std::ios::app);
        
        if (!_logFile.is_open()) {
            throw std::runtime_error("Failed to open log file: " + filename);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in FileLogHandler constructor: " << e.what() << std::endl;
        throw;
    }
}

FileLogHandler::~FileLogHandler() {
    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        if (_logFile.is_open()) {
            _logFile.close();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error in FileLogHandler destructor: " << e.what() << std::endl;
    }
}

void FileLogHandler::write(LogLevel level, const std::string& timestamp, const std::string& message) {
    std::string levelStr;
    
    switch (level) {
        case LogLevel::DEBUG:   levelStr = "DEBUG";     break;
        case LogLevel::INFO:    levelStr = "INFO";      break;
        case LogLevel::WARNING: levelStr = "WARNING";   break;
        case LogLevel::ERROR:   levelStr = "ERROR";     break;
        case LogLevel::FATAL:   levelStr = "FATAL";     break;
        default:                levelStr = "UNKNOWN";   break;
    }
    
    try {
        std::lock_guard<std::mutex> lock(_fileMutex);
        if (_logFile.is_open()) {
            _logFile << "[" << timestamp << "] [" << levelStr << "] " << message << std::endl;
            _logFile.flush();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error writing to log file: " << e.what() << std::endl;
    }
}

Logger::Logger() : _minLevel(LogLevel::INFO) {
    try {
        // Thêm ConsoleLogHandler mặc định
        // _handlers.push_back(std::make_shared<ConsoleLogHandler>());
        
        // Tạo tên file log với timestamp
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        std::tm tm_buf;
        #if defined(_WIN32) || defined(_WIN64)
            localtime_s(&tm_buf, &time);
        #else
            localtime_r(&time, &tm_buf);
        #endif
        
        std::ostringstream filename;
        filename << "logs/airlines_";
        
        char timebuf[64];
        std::strftime(timebuf, sizeof(timebuf), "%Y-%m-%d_%H-%M-%S", &tm_buf);
        filename << timebuf << ".log";
        
        // Thêm FileLogHandler
        _handlers.push_back(std::make_shared<FileLogHandler>(filename.str()));
        
        // Ghi thông báo khởi tạo
        info("Logger initialized");
    } catch (const std::exception& e) {
        std::cerr << "Logger initialization error: " << e.what() << std::endl;
    }
}

Logger::~Logger() {
    try {
        info("Logger shutting down");
        _handlers.clear();
    } catch (const std::exception& e) {
        std::cerr << "Logger shutdown error: " << e.what() << std::endl;
    }
}

std::shared_ptr<Logger> Logger::getInstance() {
    // Double-checked locking
    if (_instance == nullptr) {
        std::lock_guard<std::mutex> lock(_instanceMutex);
        if (_instance == nullptr) {
            _instance = std::shared_ptr<Logger>(new Logger());
        }
    }
    return _instance;
}

std::string Logger::_getTimestamp() const {
    try {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        std::tm tm_buf;
        #if defined(_WIN32) || defined(_WIN64)
            localtime_s(&tm_buf, &time);
        #else
            localtime_r(&time, &tm_buf);
        #endif
        
        // Lấy milliseconds
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch() % std::chrono::seconds(1)
        );
        
        std::ostringstream oss;
        oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
        oss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        
        // std::cout << oss.str() << "\n"; // DEBUG LOG;
        return oss.str();
    } catch (const std::exception& e) {
        std::cerr << "Error getting timestamp: " << e.what() << std::endl;
        return "Unknown time";
    }
}

std::string Logger::_getLevelString(LogLevel level) const {
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
        std::lock_guard<std::mutex> lock(_handlersMutex);
        _minLevel = level;
        
        // Thay vì gọi info(), ghi trực tiếp vào các handler
        std::string timestamp = _getTimestamp();
        std::string message = "Log level set to " + _getLevelString(level);
        
        for (const auto& handler : _handlers) {
            handler->write(LogLevel::INFO, timestamp, message);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error setting log level: " << e.what() << std::endl;
    }
}

void Logger::addHandler(std::shared_ptr<ILogHandler> handler) {
    try {
        if (handler) {
            std::lock_guard<std::mutex> lock(_handlersMutex);
            _handlers.push_back(handler);
            info("Log handler added");
        }
    } catch (const std::exception& e) {
        std::cerr << "Error adding log handler: " << e.what() << std::endl;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    try {
        // Nếu cấp độ log thấp hơn cấp độ tối thiểu, bỏ qua
        if (level < _minLevel) {
            return;
        }
        
        // std::cout << "Chuẩn bị gọi _getTime\n"; // DEBUG LOG
        std::string timestamp = _getTimestamp();
        // std::cout << timestamp << "\n"; // DEBUG LOG
        
        std::lock_guard<std::mutex> lock(_handlersMutex);
        for (const auto& handler : _handlers) {
            // std::cout << "Vào for" << "\n"; // DEBUG LOG
            handler->write(level, timestamp, message);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error logging message: " << e.what() << std::endl;
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