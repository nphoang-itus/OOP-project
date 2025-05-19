#ifndef TICKET_STATUS_H
#define TICKET_STATUS_H

#include <string>

enum class TicketStatus {
    PENDING,
    CONFIRMED,
    CHECKED_IN,
    BOARDED,
    COMPLETED,
    CANCELLED,
    REFUNDED
};

class TicketStatusUtil {
public:
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
};

#endif 