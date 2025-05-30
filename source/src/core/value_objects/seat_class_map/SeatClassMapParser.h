/**
 * @file SeatClassMapParser.h
 * @brief Định nghĩa các lớp parser để phân tích bản đồ hạng ghế
 * @author Nguyễn Phúc Hoàng
 */

#ifndef SEAT_CLASS_MAP_PARSER_H
#define SEAT_CLASS_MAP_PARSER_H

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>
#include "SeatClass.h"

/**
 * @interface ISeatClassMapParserStrategy
 * @brief Interface cho các chiến lược phân tích
 * 
 * Interface này định nghĩa giao diện chung cho các chiến lược
 * phân tích bản đồ hạng ghế khác nhau.
 */
class ISeatClassMapParserStrategy {
public:
    /**
     * @brief Destructor ảo
     */
    virtual ~ISeatClassMapParserStrategy() = default;
    
    /**
     * @brief Phân tích chuỗi đầu vào thành map hạng ghế
     * @param input Chuỗi cần phân tích
     * @return Map chứa hạng ghế và số lượng tương ứng
     */
    virtual std::unordered_map<SeatClass, int> parse(const std::string& input) = 0;
};

/**
 * @class StandardSeatClassMapParser
 * @brief Parser mặc định cho định dạng chuẩn (TÊN_HẠNG:SỐ_LƯỢNG,TÊN_HẠNG:SỐ_LƯỢNG,...)
 * 
 * Lớp này triển khai chiến lược phân tích cho định dạng chuẩn của bản đồ hạng ghế.
 */
class StandardSeatClassMapParser : public ISeatClassMapParserStrategy {
public:
    /**
     * @brief Phân tích chuỗi theo định dạng chuẩn
     * @param input Chuỗi có định dạng "TÊN_HẠNG:SỐ_LƯỢNG,TÊN_HẠNG:SỐ_LƯỢNG,..."
     * @return Map chứa hạng ghế và số lượng tương ứng
     */
    std::unordered_map<SeatClass, int> parse(const std::string& input) override {
        std::unordered_map<SeatClass, int> classMap;
        std::string value = input;
        std::string delimiter = ",";
        size_t pos = 0;
        std::string token;
        
        while ((pos = value.find(delimiter)) != std::string::npos) {
            token = value.substr(0, pos);
            parseClassEntry(token, classMap);
            value.erase(0, pos + delimiter.length());
        }
        parseClassEntry(value, classMap);
        
        return classMap;
    }

private:
    /**
     * @brief Phân tích một mục hạng ghế
     * @param entry Mục hạng ghế có định dạng "TÊN_HẠNG:SỐ_LƯỢNG"
     * @param classMap Map để lưu kết quả phân tích
     */
    void parseClassEntry(const std::string& entry, std::unordered_map<SeatClass, int>& classMap) {
        size_t colonPos = entry.find(':');
        if (colonPos != std::string::npos) {
            std::string className = entry.substr(0, colonPos);
            std::string seatCount = entry.substr(colonPos + 1);

            // Convert class name to uppercase
            std::string uppercaseClassName = className;
            std::transform(uppercaseClassName.begin(), uppercaseClassName.end(), 
                          uppercaseClassName.begin(), ::toupper);

            if (auto seatClass = SeatClassRegistry::getByName(uppercaseClassName)) {
                classMap[*seatClass] = std::stoi(seatCount);
            }
        }
    }
};

/**
 * @class SeatClassMapParserFactory
 * @brief Factory để quản lý các chiến lược phân tích khác nhau
 * 
 * Lớp này cung cấp cơ chế đăng ký và lấy các chiến lược phân tích
 * bản đồ hạng ghế khác nhau.
 */
class SeatClassMapParserFactory {
private:
    inline static std::unordered_map<std::string, std::shared_ptr<ISeatClassMapParserStrategy>> _strategies; ///< Map lưu trữ các chiến lược
    inline static bool _initialized = false; ///< Cờ khởi tạo

    /**
     * @brief Khởi tạo factory với các chiến lược mặc định
     */
    static void initialize() {
        if (_initialized) return;
        
        // Register default parser strategy
        registerStrategy("standard", std::make_shared<StandardSeatClassMapParser>());
        
        _initialized = true;
    }

public:
    /**
     * @brief Đăng ký một chiến lược phân tích mới
     * @param name Tên chiến lược
     * @param strategy Con trỏ đến chiến lược
     */
    static void registerStrategy(const std::string& name, std::shared_ptr<ISeatClassMapParserStrategy> strategy) {
        _strategies[name] = std::move(strategy);
    }

    /**
     * @brief Lấy chiến lược phân tích theo tên
     * @param name Tên chiến lược
     * @return Con trỏ đến chiến lược, hoặc chiến lược mặc định nếu không tìm thấy
     */
    static std::shared_ptr<ISeatClassMapParserStrategy> getStrategy(const std::string& name) {
        initialize();
        auto it = _strategies.find(name);
        if (it != _strategies.end()) {
            return it->second;
        }
        return _strategies["standard"]; // Return default strategy if not found
    }
};

/**
 * @class SeatClassMapParser
 * @brief Lớp parser chính sử dụng các chiến lược
 * 
 * Lớp này cung cấp giao diện đơn giản để phân tích bản đồ hạng ghế
 * bằng cách sử dụng các chiến lược khác nhau.
 */
class SeatClassMapParser {
public:
    /**
     * @brief Phân tích chuỗi bản đồ hạng ghế
     * @param value Chuỗi cần phân tích
     * @param strategyName Tên chiến lược phân tích (mặc định là "standard")
     * @return Map chứa hạng ghế và số lượng tương ứng
     */
    static std::unordered_map<SeatClass, int> parse(const std::string& value, const std::string& strategyName = "standard") {
        auto strategy = SeatClassMapParserFactory::getStrategy(strategyName);
        return strategy->parse(value);
    }
};

#endif