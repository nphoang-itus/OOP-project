/**
 * @file Aircraft.h
 * @brief Thực thể Aircraft đại diện cho máy bay vật lý trong hệ thống quản lý chuyến bay
 * @author Đội phát triển Hệ thống Quản lý Chuyến bay
 * @version 1.0
 */

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "IEntity.h"
#include "../value_objects/aircraft_serial/AircraftSerial.h"
#include "../value_objects/seat_class_map/SeatClassMap.h"
#include "../exceptions/Result.h"
#include <memory>

using AircraftModel = std::string; ///< Type alias cho chuỗi model máy bay

/**
 * @brief Đại diện cho một máy bay vật lý với thông số kỹ thuật và cấu hình chỗ ngồi
 * 
 * Thực thể Aircraft đóng gói tất cả thông tin về một máy bay vật lý bao gồm
 * số serial duy nhất, kiểu model và bố cục chỗ ngồi chi tiết. Nó cung cấp validation
 * cho dữ liệu máy bay và logic nghiệp vụ cho các thao tác quản lý chỗ ngồi.
 * 
 * Các instance Aircraft được tạo thông qua các phương thức factory đảm bảo tính hợp lệ của dữ liệu.
 * Bố cục chỗ ngồi định nghĩa các hạng ghế có sẵn và sức chứa của chúng.
 */
class Aircraft : public IEntity {
protected:
    AircraftSerial _serial;    ///< Số serial máy bay duy nhất
    AircraftModel _model;      ///< Model/kiểu máy bay (ví dụ: "Boeing 737", "Airbus A320")
    SeatClassMap _seatLayout;  ///< Cấu hình chỗ ngồi ánh xạ các hạng ghế đến số lượng ghế

    /**
     * @brief Constructor được bảo vệ để tạo instance máy bay
     * @param serial Số serial máy bay duy nhất
     * @param model Model/kiểu máy bay
     * @param seatLayout Cấu hình chỗ ngồi cho máy bay
     */
    Aircraft(AircraftSerial serial, AircraftModel model, SeatClassMap seatLayout)
        : _serial(std::move(serial)), _model(std::move(model)), _seatLayout(std::move(seatLayout)) {}

public:
    /**
     * @brief Tạo máy bay từ các value object đã được validate
     * @param serial Số serial máy bay đã được validate
     * @param model Chuỗi model máy bay (không được rỗng)
     * @param seatLayout Cấu hình bố cục chỗ ngồi đã được validate
     * @return Result chứa instance Aircraft hoặc chi tiết lỗi
     * @throw CoreError nếu model rỗng
     */
    static Result<Aircraft> create(const AircraftSerial& serial, const AircraftModel& model, const SeatClassMap& seatLayout) {
        if (model.empty()) {
            return Failure<Aircraft>(CoreError("Aircraft model cannot be empty", "INVALID_MODEL"));
        }
        return Success(Aircraft(serial, model, seatLayout));
    }

    /**
     * @brief Tạo máy bay từ đầu vào chuỗi với validation
     * @param serial Số serial máy bay dưới dạng chuỗi
     * @param model Chuỗi model máy bay (không được rỗng)
     * @param seatLayout Cấu hình bố cục chỗ ngồi dưới dạng chuỗi
     * @return Result chứa instance Aircraft hoặc chi tiết lỗi
     * @throw CoreError nếu bất kỳ đầu vào nào không hợp lệ
     */
    static Result<Aircraft> create(const std::string& serial, const std::string& model, const std::string& seatLayout) {
        auto serialResult = AircraftSerial::create(serial);
        if (!serialResult) {
            return Failure<Aircraft>(CoreError("Invalid aircraft serial: " + serialResult.error().message, "INVALID_SERIAL"));
        }

        if (model.empty()) {
            return Failure<Aircraft>(CoreError("Aircraft model cannot be empty", "INVALID_MODEL"));
        }

        auto seatLayoutResult = SeatClassMap::create(seatLayout);
        if (!seatLayoutResult) {
            return Failure<Aircraft>(CoreError("Invalid seat layout: " + seatLayoutResult.error().message, "INVALID_SEAT_LAYOUT"));
        }

        return Success(Aircraft(*serialResult, model, *seatLayoutResult));
    }

    /**
     * @brief Lấy ID được tạo bởi cơ sở dữ liệu
     * @return ID cơ sở dữ liệu số nguyên
     */
    int getId() const override { return _id; }

    /**
     * @brief Lấy số serial máy bay dưới dạng string ID
     * @return Chuỗi số serial máy bay
     */
    std::string getStringId() const override { return _serial.toString(); }

    /**
     * @brief Lấy số serial máy bay
     * @return Tham chiếu đến value object AircraftSerial
     */
    const AircraftSerial& getSerial() const { return _serial; }

    /**
     * @brief Lấy model máy bay
     * @return Tham chiếu đến chuỗi model máy bay
     */
    const AircraftModel& getModel() const { return _model; }

    /**
     * @brief Lấy cấu hình bố cục chỗ ngồi
     * @return Tham chiếu đến SeatClassMap chứa cấu hình chỗ ngồi
     */
    const SeatClassMap& getSeatLayout() const { return _seatLayout; }

    /**
     * @brief Đặt ID cơ sở dữ liệu
     * @param id ID được tạo bởi cơ sở dữ liệu để gán
     */
    void setId(int id) override { _id = id; }

    /**
     * @brief Cập nhật model máy bay
     * @param model Chuỗi model máy bay mới
     */
    void setModel(const std::string& model) { _model = model; }

    /**
     * @brief Cập nhật cấu hình bố cục chỗ ngồi
     * @param seatLayout Cấu hình bố cục chỗ ngồi mới
     */
    void setSeatLayout(const SeatClassMap& seatLayout) { _seatLayout = seatLayout; }

    /**
     * @brief Lấy biểu diễn chuỗi của máy bay
     * @return Chuỗi định dạng chứa tất cả thuộc tính máy bay
     */
    std::string toString() const override {
        return "Aircraft{id=" + std::to_string(_id) + 
               ", serial=" + _serial.toString() + 
               ", model=" + _model + 
               ", seatLayout=" + _seatLayout.toString() + "}";
    }

    /**
     * @brief Kiểm tra tính bằng nhau với thực thể khác
     * @param other Thực thể để so sánh
     * @return true nếu cả hai đều là thực thể Aircraft với cùng ID cơ sở dữ liệu
     */
    bool equals(const IEntity& other) const override {
        if (const auto* aircraft = dynamic_cast<const Aircraft*>(&other)) {
            return _id == aircraft->_id;
        }
        return false;
    }

    /**
     * @brief Tạo bản sao sâu của máy bay
     * @return Unique pointer đến instance Aircraft mới với giá trị giống hệt
     */
    std::unique_ptr<IEntity> clone() const override {
        auto clone = std::unique_ptr<Aircraft>(new Aircraft(_serial, _model, _seatLayout));
        clone->_id = _id;
        return clone;
    }

    /**
     * @brief Kiểm tra xem máy bay có hạng ghế cụ thể không
     * @param seatClassCode Mã hạng ghế để kiểm tra
     * @return true nếu máy bay có hạng ghế được chỉ định
     */
    bool hasSeatClass(const std::string& seatClassCode) const {
        return _seatLayout.hasSeatClass(seatClassCode);
    }

    /**
     * @brief Lấy tổng số ghế cho một hạng cụ thể
     * @param seatClassCode Mã hạng ghế để truy vấn
     * @return Số ghế có sẵn trong hạng được chỉ định
     */
    int getSeatCount(const std::string& seatClassCode) const {
        return _seatLayout.getSeatCount(seatClassCode);
    }

    /**
     * @brief Validate xem số ghế có hợp lệ cho máy bay này không
     * @param seatNumber Chuỗi số ghế để validate
     * @return true nếu số ghế tồn tại trong cấu hình máy bay
     */
    bool isValidSeatNumber(const std::string& seatNumber) const {
        return _seatLayout.isValidSeatNumber(seatNumber);
    }
};

#endif