/**
 * @file Logger.h
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-01
 * 
 * @copyright Copyright (c) 2025
 * 
 */

 #ifndef LOGGER_H
 #define LOGGER_H
 
 #include <mutex>
 #include <fstream>
 #include <string>
 
 /**
  * @enum LogLevel
  * @brief Các mức độ log
  */
 enum class LogLevel {
     DEBUG,   ///< Thông tin gỡ lỗi
     INFO,    ///< Thông tin chung
     WARNING, ///< Cảnh báo
     ERROR,   ///< Lỗi
     FATAL    ///< Lỗi nghiêm trọng
 };
 
 /**
  * @class Logger
  * @brief Lớp xử lý ghi log
  */
 class Logger {
 private:
     static Logger* _instance; ///< Thể hiện duy nhất của Logger
     static std::mutex _mutex; ///< Mutex để đồng bộ hóa
     
     std::ofstream _logFile; ///< File log
     LogLevel minLevel; ///< Mức độ log tối thiểu
     
     /**
     * @brief Constructor
     */
     Logger();
     
     /**
     * @brief Destructor
     */
     ~Logger();
     
     /**
     * @brief Lấy thời gian hiện tại dưới dạng chuỗi
     * @return Chuỗi thời gian
     */
     std::string _getTimestamp();
     
     /**
     * @brief Chuyển đổi mức độ log thành chuỗi
     * @param level Mức độ log
     * @return Chuỗi mức độ log
     */
     std::string _getLevelString(LogLevel level);
 
 public:
     /**
     * @brief Lấy thể hiện duy nhất của Logger
     * @return Con trỏ đến thể hiện Logger
     */
     static Logger* getInstance();
     
     /**
     * @brief Xóa constructor copy
     */
     Logger(const Logger&) = delete;
     
     /**
     * @brief Xóa toán tử gán
     */
     Logger& operator=(const Logger&) = delete;
     
     /**
     * @brief Thiết lập mức độ log tối thiểu
     * @param level Mức độ log tối thiểu
     */
     void setMinLevel(LogLevel level);
     
     /**
     * @brief Ghi log
     * @param level Mức độ log
     * @param message Thông báo log
     */
     void log(LogLevel level, const std::string& message);
     
     /**
     * @brief Ghi log ở mức độ DEBUG
     * @param message Thông báo log
     */
     void debug(const std::string& message);
     
     /**
     * @brief Ghi log ở mức độ INFO
     * @param message Thông báo log
     */
     void info(const std::string& message);
     
     /**
     * @brief Ghi log ở mức độ WARNING
     * @param message Thông báo log
     */
     void warning(const std::string& message);
     
     /**
     * @brief Ghi log ở mức độ ERROR
     * @param message Thông báo log
     */
     void error(const std::string& message);
     
     /**
     * @brief Ghi log ở mức độ FATAL
     * @param message Thông báo log
     */
     void fatal(const std::string& message);
 };
 
 #endif