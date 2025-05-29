/**
 * @file TicketStatus.h
 * @brief Enum và utility cho trạng thái vé máy bay trong hệ thống quản lý chuyến bay
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef TICKET_STATUS_H
#define TICKET_STATUS_H

#include <string>

/**
 * @brief Enum định nghĩa các trạng thái có thể có của vé máy bay
 * 
 * TicketStatus đại diện cho vòng đời của một vé máy bay từ lúc được đặt
 * cho đến khi hoàn thành chuyến bay hoặc bị hủy/hoàn tiền. Các trạng thái này
 * được sử dụng để kiểm soát quy trình check-in, boarding và các thao tác khác.
 */
enum class TicketStatus {
    PENDING,        ///< Chờ xác nhận - Vé vừa được đặt, chưa thanh toán hoặc xác nhận
    CONFIRMED,      ///< Đã xác nhận - Vé đã được thanh toán và xác nhận thành công
    CHECKED_IN,     ///< Đã check-in - Hành khách đã hoàn tất thủ tục check-in
    BOARDED,        ///< Đã lên máy bay - Hành khách đã lên máy bay thành công
    COMPLETED,      ///< Hoàn thành - Chuyến bay đã kết thúc, hành khách đã đến đích
    CANCELLED,      ///< Đã hủy - Vé bị hủy bởi hành khách hoặc hãng hàng không
    REFUNDED        ///< Đã hoàn tiền - Vé đã được hoàn tiền thành công
};

/**
 * @brief Lớp utility cung cấp các phương thức chuyển đổi cho TicketStatus
 * 
 * TicketStatusUtil cung cấp các phương thức để chuyển đổi giữa enum TicketStatus
 * và các biểu diễn chuỗi khác nhau (tiếng Anh và tiếng Việt) để hiển thị
 * và lưu trữ dữ liệu, cũng như parsing từ database.
 */
class TicketStatusUtil {
public:
    /**
     * @brief Chuyển đổi TicketStatus thành chuỗi tiếng Anh
     * @param status Trạng thái vé cần chuyển đổi
     * @return Chuỗi tiếng Anh đại diện cho trạng thái (ví dụ: "PENDING", "CONFIRMED")
     */
    static std::string toString(TicketStatus status) {
        switch (status) {
            case TicketStatus::PENDING: return "PENDING";
            case TicketStatus::CONFIRMED: return "CONFIRMED";
            case TicketStatus::CHECKED_IN: return "CHECKED_IN";
            case TicketStatus::BOARDED: return "BOARDED";
            case TicketStatus::COMPLETED: return "COMPLETED";
            case TicketStatus::CANCELLED: return "CANCELLED";
            case TicketStatus::REFUNDED: return "REFUNDED";
            default: return "UNKNOWN";
        }
    }

    /**
     * @brief Chuyển đổi TicketStatus thành chuỗi tiếng Việt
     * @param status Trạng thái vé cần chuyển đổi
     * @return Chuỗi tiếng Việt đại diện cho trạng thái (ví dụ: "Chờ xác nhận", "Đã xác nhận")
     */
    static std::string toVietnamese(TicketStatus status) {
        switch (status) {
            case TicketStatus::PENDING: return "Chờ xác nhận";
            case TicketStatus::CONFIRMED: return "Đã xác nhận";
            case TicketStatus::CHECKED_IN: return "Đã check-in";
            case TicketStatus::BOARDED: return "Đã lên máy bay";
            case TicketStatus::COMPLETED: return "Hoàn thành";
            case TicketStatus::CANCELLED: return "Đã hủy";
            case TicketStatus::REFUNDED: return "Đã hoàn tiền";
            default: return "Không xác định";
        }
    }
    
    /**
     * @brief Chuyển đổi chuỗi tiếng Anh thành TicketStatus
     * @param statusString Chuỗi tiếng Anh đại diện cho trạng thái
     * @return TicketStatus tương ứng
     * @throw std::invalid_argument nếu chuỗi không hợp lệ
     */
    static TicketStatus fromString(const std::string& statusString) {
        if (statusString == "PENDING") return TicketStatus::PENDING;
        else if (statusString == "CONFIRMED") return TicketStatus::CONFIRMED;
        else if (statusString == "CHECKED_IN") return TicketStatus::CHECKED_IN;
        else if (statusString == "BOARDED") return TicketStatus::BOARDED;
        else if (statusString == "COMPLETED") return TicketStatus::COMPLETED;
        else if (statusString == "CANCELLED") return TicketStatus::CANCELLED;
        else if (statusString == "REFUNDED") return TicketStatus::REFUNDED;
        else throw std::invalid_argument("Unknown ticket status: " + statusString);
    }
};

#endif