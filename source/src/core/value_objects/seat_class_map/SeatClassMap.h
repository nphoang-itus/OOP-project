/**
 * @file SeatClassMap.h
 * @brief Định nghĩa lớp value object SeatClassMap
 * @author Nhóm dự án OOP
 */

#ifndef SEAT_CLASS_MAP_H
#define SEAT_CLASS_MAP_H

#include <string>
#include <map>
#include <sstream>
#include "SeatClass.h"
#include "../../exceptions/Result.h"

/**
 * @brief Toán tử so sánh cho SeatClass
 * @param lhs SeatClass bên trái
 * @param rhs SeatClass bên phải
 * @return true nếu mã của lhs nhỏ hơn mã của rhs
 */
inline bool operator<(const SeatClass &lhs, const SeatClass &rhs)
{
    return lhs.getCode() < rhs.getCode();
}

/**
 * @class SeatClassMap
 * @brief Đại diện cho bản đồ các hạng ghế và số lượng tương ứng
 * 
 * Lớp này là một value object đóng gói thông tin về các hạng ghế
 * và số lượng ghế của từng hạng trong máy bay.
 */
class SeatClassMap
{
private:
    std::map<SeatClass, int> _seatCounts; ///< Map lưu trữ hạng ghế và số lượng tương ứng

public:
    /**
     * @brief Constructor mặc định công khai
     */
    SeatClassMap() = default;

    /**
     * @brief Toán tử bằng
     * @param other SeatClassMap khác để so sánh
     * @return true nếu hai bản đồ hạng ghế bằng nhau, false nếu ngược lại
     */
    bool operator==(const SeatClassMap &other) const
    {
        return _seatCounts == other._seatCounts;
    }

    /**
     * @brief Toán tử không bằng
     * @param other SeatClassMap khác để so sánh
     * @return true nếu hai bản đồ hạng ghế không bằng nhau, false nếu ngược lại
     */
    bool operator!=(const SeatClassMap &other) const
    {
        return !(*this == other);
    }

    /**
     * @brief Tạo SeatClassMap từ map số lượng ghế
     * @param seatCounts Map chứa hạng ghế và số lượng
     * @return Result chứa SeatClassMap
     */
    static Result<SeatClassMap> create(const std::map<SeatClass, int> &seatCounts)
    {
        SeatClassMap map;
        map._seatCounts = seatCounts;
        return Success(map);
    }

    /**
     * @brief Tạo SeatClassMap từ chuỗi mô tả bố trí ghế
     * @param seatLayoutStr Chuỗi có định dạng "E:100,B:20,F:10"
     * @return Result chứa SeatClassMap
     */
    static Result<SeatClassMap> create(const std::string &seatLayoutStr)
    {
        SeatClassMap map;
        if (seatLayoutStr.empty())
        {
            return Success(map);
        }

        // Parse string format: "E:100,B:20,F:10"
        std::stringstream ss(seatLayoutStr);
        std::string item;
        while (std::getline(ss, item, ','))
        {
            size_t pos = item.find(':');
            if (pos == std::string::npos)
                continue;

            std::string code = item.substr(0, pos);
            int count = std::stoi(item.substr(pos + 1));

            auto seatClassResult = SeatClassRegistry::getByCode(code);
            if (!seatClassResult)
                continue;

            map._seatCounts[seatClassResult.value()] = count;
        }
        return Success(map);
    }

    /**
     * @brief Kiểm tra xem có hạng ghế nào đó không
     * @param seatClassCode Mã hạng ghế cần kiểm tra
     * @return true nếu có hạng ghế, false nếu không
     */
    bool hasSeatClass(const std::string &seatClassCode) const
    {
        auto seatClassResult = SeatClassRegistry::getByCode(seatClassCode);
        if (!seatClassResult)
            return false;
        return _seatCounts.find(seatClassResult.value()) != _seatCounts.end();
    }

    /**
     * @brief Lấy số lượng ghế của một hạng
     * @param seatClassCode Mã hạng ghế
     * @return Số lượng ghế của hạng đó, 0 nếu không tìm thấy
     */
    int getSeatCount(const std::string &seatClassCode) const
    {
        auto seatClassResult = SeatClassRegistry::getByCode(seatClassCode);
        if (!seatClassResult)
            return 0;
        auto it = _seatCounts.find(seatClassResult.value());
        return it != _seatCounts.end() ? it->second : 0;
    }

    /**
     * @brief Kiểm tra tính hợp lệ của số ghế
     * @param seatNumber Số ghế cần kiểm tra
     * @return true nếu số ghế hợp lệ, false nếu không
     */
    bool isValidSeatNumber(const std::string &seatNumber) const
    {
        if (seatNumber.length() < 2)
            return false;
        std::string code = seatNumber.substr(0, 1);
        std::string number = seatNumber.substr(1);
        return hasSeatClass(code) && std::stoi(number) <= getSeatCount(code);
    }

    /**
     * @brief Chuyển đổi bản đồ hạng ghế thành chuỗi
     * @return Chuỗi mô tả bản đồ hạng ghế
     */
    std::string toString() const
    {
        std::stringstream ss;
        for (const auto &[seatClass, count] : _seatCounts)
        {
            if (!ss.str().empty())
                ss << ",";
            ss << seatClass.getName() << ":" << count;
        }
        return ss.str();
    }

    /**
     * @brief Lấy map số lượng ghế
     * @return Reference đến map chứa hạng ghế và số lượng
     */
    const std::map<SeatClass, int> &getSeatCounts() const { return _seatCounts; }

    /**
     * @brief Lấy tổng số ghế
     * @return Tổng số ghế của tất cả hạng
     */
    int getTotalSeatCount() const {
        int total = 0;
        for (const auto &[seatClass, count] : _seatCounts) {
            total += count;
        }
        return total;
    }
};

#endif
