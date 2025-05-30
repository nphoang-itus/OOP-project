/**
 * @file Ticket.h
 * @brief Thực thể Ticket đại diện cho đặt chỗ và booking chuyến bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef TICKET_H
#define TICKET_H

#include "IEntity.h"
#include "Passenger.h"
#include "Flight.h"
#include "../value_objects/ticket_number/TicketNumber.h"
#include "../value_objects/seat_number/SeatNumber.h"
#include "../value_objects/ticket_status/TicketStatus.h"
#include "../value_objects/flight_status/FlightStatus.h"
#include "../value_objects/price/Price.h"
#include "../exceptions/Result.h"
#include <memory>
#include <string>
#include <sstream>

/**
 * @brief Đại diện cho vé chuyến bay liên kết hành khách với chuyến bay và ghế cụ thể
 * 
 * Thực thể Ticket là bản ghi booking cốt lõi kết hợp hành khách với một
 * chuyến bay cụ thể, ghế được gán và thông tin giá cả. Nó quản lý chu kỳ vé hoàn chỉnh
 * từ booking ban đầu đến hoàn thành hoặc hủy bỏ.
 * 
 * Vé theo dõi trạng thái của chúng qua các giai đoạn khác nhau (chờ xử lý, đã xác nhận, đã check-in, v.v.)
 * và cung cấp logic nghiệp vụ để xác định chuyển đổi trạng thái hợp lệ dựa trên
 * trạng thái chuyến bay và vé hiện tại.
 */
class Ticket : public IEntity {
protected:
    TicketNumber _ticketNumber;              ///< Định danh vé duy nhất
    std::shared_ptr<Passenger> _passenger;   ///< Hành khách sở hữu vé này
    std::shared_ptr<Flight> _flight;         ///< Chuyến bay mà vé này dành cho
    SeatNumber _seatNumber;                  ///< Ghế được gán trên chuyến bay
    Price _price;                            ///< Thông tin giá vé
    TicketStatus _status;                    ///< Trạng thái vé hiện tại

    /**
     * @brief Constructor được bảo vệ để tạo instance vé
     * @param ticketNumber Định danh vé duy nhất
     * @param passenger Hành khách sở hữu vé
     * @param flight Chuyến bay mà vé dành cho
     * @param seatNumber Số ghế được gán
     * @param price Thông tin giá vé
     */
    Ticket(TicketNumber ticketNumber, std::shared_ptr<Passenger> passenger,
           std::shared_ptr<Flight> flight, SeatNumber seatNumber, Price price)
        : _ticketNumber(std::move(ticketNumber))
        , _passenger(std::move(passenger))
        , _flight(std::move(flight))
        , _seatNumber(std::move(seatNumber))
        , _price(std::move(price))
        , _status(TicketStatus::PENDING) {
    }

public:
    /**
     * @brief Tạo vé từ các value object đã được validate
     * @param ticketNumber Số vé đã được validate
     * @param passenger Thực thể Passenger (không được null)
     * @param flight Thực thể Flight (không được null)
     * @param seatNumber Số ghế đã được validate
     * @param price Thông tin giá đã được validate
     * @return Result chứa instance Ticket hoặc chi tiết lỗi
     * @throw CoreError nếu passenger hoặc flight là null
     */
    static Result<Ticket> create(const TicketNumber& ticketNumber, std::shared_ptr<Passenger> passenger,
                               std::shared_ptr<Flight> flight, const SeatNumber& seatNumber, const Price& price) {
        if (!passenger) {
            return Failure<Ticket>(CoreError("Passenger cannot be null", "INVALID_PASSENGER"));
        }
        if (!flight) {
            return Failure<Ticket>(CoreError("Flight cannot be null", "INVALID_FLIGHT"));
        }
        return Success(Ticket(ticketNumber, passenger, flight, seatNumber, price));
    }

    /**
     * @brief Tạo vé từ đầu vào chuỗi với validation
     * @param ticketNumber Số vé dưới dạng chuỗi
     * @param passenger Thực thể Passenger (không được null)
     * @param flight Thực thể Flight (không được null)
     * @param seatNumber Số ghế dưới dạng chuỗi
     * @param price Giá dưới dạng chuỗi
     * @return Result chứa instance Ticket hoặc chi tiết lỗi
     * @throw CoreError nếu bất kỳ đầu vào nào không hợp lệ hoặc các thực thể là null
     */
    static Result<Ticket> create(const std::string& ticketNumber, std::shared_ptr<Passenger> passenger,
                               std::shared_ptr<Flight> flight, const std::string& seatNumber, const std::string& price) {
        auto ticketNumberResult = TicketNumber::create(ticketNumber);
        if (!ticketNumberResult) {
            return Failure<Ticket>(CoreError("Invalid ticket number: " + ticketNumberResult.error().message, "INVALID_TICKET_NUMBER"));
        }

        if (!passenger) {
            return Failure<Ticket>(CoreError("Passenger cannot be null", "INVALID_PASSENGER"));
        }

        if (!flight) {
            return Failure<Ticket>(CoreError("Flight cannot be null", "INVALID_FLIGHT"));
        }

        auto seatNumberResult = SeatNumber::create(seatNumber, flight->getAircraft()->getSeatLayout());
        if (!seatNumberResult) {
            return Failure<Ticket>(CoreError("Invalid seat number: " + seatNumberResult.error().message, "INVALID_SEAT_NUMBER"));
        }

        auto priceResult = Price::create(price);
        if (!priceResult) {
            return Failure<Ticket>(CoreError("Invalid price: " + priceResult.error().message, "INVALID_PRICE"));
        }

        return Success(Ticket(*ticketNumberResult, passenger, flight, *seatNumberResult, *priceResult));
    }

    /**
     * @brief Cập nhật trạng thái vé
     * @param status Trạng thái vé mới để đặt
     */
    void setStatus(TicketStatus status) {
        _status = status;
    }

    /**
     * @brief Lấy trạng thái vé hiện tại
     * @return Giá trị enum TicketStatus hiện tại
     */
    TicketStatus getStatus() const {
        return _status;
    }

    /**
     * @brief Lấy trạng thái vé dưới dạng chuỗi tiếng Anh
     * @return Chuỗi trạng thái có thể đọc được bằng tiếng Anh
     */
    std::string getStatusString() const {
        return TicketStatusUtil::toString(_status);
    }

    /**
     * @brief Lấy trạng thái vé bằng tiếng Việt
     * @return Chuỗi trạng thái có thể đọc được bằng tiếng Việt
     */
    std::string getStatusVietnamese() const {
        return TicketStatusUtil::toVietnamese(_status);
    }

    /**
     * @brief Lấy ID được tạo bởi cơ sở dữ liệu
     * @return ID cơ sở dữ liệu số nguyên
     */
    int getId() const override {
        return _id;
    }

    /**
     * @brief Lấy số vé dưới dạng string ID
     * @return Chuỗi số vé phục vụ như định danh nghiệp vụ
     */
    std::string getStringId() const override {
        return _ticketNumber.toString();
    }

    /**
     * @brief Đặt ID cơ sở dữ liệu
     * @param id ID được tạo bởi cơ sở dữ liệu để gán
     */
    void setId(int id) override {
        _id = id;
    }

    /**
     * @brief Lấy biểu diễn chuỗi của vé
     * @return Chuỗi định dạng chứa tất cả thuộc tính vé
     */
    std::string toString() const override {
        return "Ticket{id=" + std::to_string(_id) + 
               ", ticketNumber=" + _ticketNumber.toString() + 
               ", passenger=" + _passenger->toString() + 
               ", flight=" + _flight->toString() + 
               ", seatNumber=" + _seatNumber.toString() + 
               ", price=" + _price.toString() + 
               ", status=" + getStatusString() + "}";
    }

    /**
     * @brief Kiểm tra tính bằng nhau với thực thể khác
     * @param other Thực thể để so sánh
     * @return true nếu cả hai đều là thực thể Ticket với cùng ID cơ sở dữ liệu
     */
    bool equals(const IEntity& other) const override {
        if (const auto* ticket = dynamic_cast<const Ticket*>(&other)) {
            return _id == ticket->_id;
        }
        return false;
    }

    /**
     * @brief Tạo bản sao sâu của vé
     * @return Unique pointer đến instance Ticket mới với giá trị giống hệt
     */
    std::unique_ptr<IEntity> clone() const override {
        auto clone = std::unique_ptr<Ticket>(new Ticket(_ticketNumber, _passenger, _flight, _seatNumber, _price));
        clone->_id = _id;
        clone->_status = _status;
        return clone;
    }

    /**
     * @brief Lấy số vé
     * @return Tham chiếu đến value object TicketNumber
     */
    const TicketNumber& getTicketNumber() const { return _ticketNumber; }

    /**
     * @brief Lấy hành khách
     * @return Shared pointer đến thực thể Passenger
     */
    const std::shared_ptr<Passenger>& getPassenger() const { return _passenger; }

    /**
     * @brief Lấy chuyến bay
     * @return Shared pointer đến thực thể Flight
     */
    const std::shared_ptr<Flight>& getFlight() const { return _flight; }

    /**
     * @brief Lấy số ghế được gán
     * @return Tham chiếu đến value object SeatNumber
     */
    const SeatNumber& getSeatNumber() const { return _seatNumber; }

    /**
     * @brief Lấy giá vé
     * @return Tham chiếu đến value object Price
     */
    const Price& getPrice() const { return _price; }

    /**
     * @brief Cập nhật giá vé
     * @param price Thông tin giá mới
     */
    void setPrice(const Price& price) { _price = price; }

    /**
     * @brief Kiểm tra xem vé có thể bị hủy không
     * @return true nếu vé ở trạng thái PENDING hoặc CONFIRMED
     */
    bool canBeCancelled() const {
        return _status == TicketStatus::PENDING || _status == TicketStatus::CONFIRMED;
    }

    /**
     * @brief Kiểm tra xem vé có đủ điều kiện hoàn tiền không
     * @return true nếu vé đã được xác nhận và chuyến bay chưa khởi hành
     */
    bool canBeRefunded() const {
        return _status == TicketStatus::CONFIRMED && 
               _flight->getStatus() != FlightStatus::CANCELLED &&
               _flight->getStatus() != FlightStatus::DEPARTED &&
               _flight->getStatus() != FlightStatus::IN_FLIGHT &&
               _flight->getStatus() != FlightStatus::LANDED;
    }

    /**
     * @brief Kiểm tra xem hành khách có thể check-in cho vé này không
     * @return true nếu vé đã được xác nhận và chuyến bay đang boarding
     */
    bool canBeCheckedIn() const {
        return _status == TicketStatus::CONFIRMED && 
               _flight->getStatus() == FlightStatus::BOARDING;
    }

    /**
     * @brief Kiểm tra xem hành khách có thể lên máy bay với vé này không
     * @return true nếu vé đã check-in và chuyến bay đang boarding
     */
    bool canBeBoarded() const {
        return _status == TicketStatus::CHECKED_IN && 
               _flight->getStatus() == FlightStatus::BOARDING;
    }

    /**
     * @brief Kiểm tra xem vé có thể được đánh dấu là hoàn thành không
     * @return true nếu hành khách đã lên máy bay và chuyến bay đã hạ cánh
     */
    bool canBeCompleted() const {
        return _status == TicketStatus::BOARDED && 
               _flight->getStatus() == FlightStatus::LANDED;
    }
};

#endif