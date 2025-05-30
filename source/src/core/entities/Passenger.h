/**
 * @file Passenger.h
 * @brief Thực thể Passenger đại diện cho các cá nhân có thể đặt vé và đi du lịch trên chuyến bay
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef PASSENGER_H
#define PASSENGER_H

#include "IEntity.h"
#include "../value_objects/contact_info/ContactInfo.h"
#include "../value_objects/passport_number/PassportNumber.h"
#include "../exceptions/Result.h"

using Name = std::string; ///< Type alias cho chuỗi tên hành khách

/**
 * @brief Đại diện cho hành khách với thông tin cá nhân và giấy tờ du lịch
 * 
 * Thực thể Passenger đóng gói tất cả thông tin cần thiết về một cá nhân
 * có thể đặt chuyến bay và đi du lịch. Điều này bao gồm tên của họ, thông tin liên lạc,
 * và chi tiết hộ chiếu để xác minh du lịch quốc tế.
 * 
 * Các instance Passenger được tạo thông qua các phương thức factory validate tất cả dữ liệu đầu vào.
 * Số hộ chiếu phục vụ như định danh nghiệp vụ duy nhất cho hành khách.
 */
class Passenger : public IEntity {
protected:
    Name _name;                ///< Họ tên đầy đủ của hành khách
    ContactInfo _contactInfo;  ///< Thông tin liên lạc (email, điện thoại, v.v.)
    PassportNumber _passport;  ///< Số hộ chiếu để định danh du lịch

    /**
     * @brief Constructor được bảo vệ để tạo instance hành khách
     * @param name Họ tên đầy đủ của hành khách
     * @param contactInfo Thông tin liên lạc
     * @param passport Số hộ chiếu
     */
    Passenger(Name name, ContactInfo contactInfo, PassportNumber passport)
        : _name(std::move(name)), _contactInfo(std::move(contactInfo)), _passport(std::move(passport)) {}

public:
    /**
     * @brief Tạo hành khách từ các value object đã được validate
     * @param name Họ tên đầy đủ của hành khách (không được rỗng)
     * @param contactInfo Thông tin liên lạc đã được validate
     * @param passport Số hộ chiếu đã được validate
     * @return Result chứa instance Passenger hoặc chi tiết lỗi
     * @throw CoreError nếu tên rỗng
     */
    static Result<Passenger> create(const Name& name, const ContactInfo& contactInfo, const PassportNumber& passport) {
        if (name.empty()) {
            return Failure<Passenger>(CoreError("Passenger name cannot be empty", "INVALID_NAME"));
        }
        return Success(Passenger(name, contactInfo, passport));
    }

    /**
     * @brief Tạo hành khách từ đầu vào chuỗi với validation
     * @param name Họ tên đầy đủ của hành khách (không được rỗng)
     * @param contactInfo Thông tin liên lạc dưới dạng chuỗi
     * @param passport Số hộ chiếu dưới dạng chuỗi
     * @return Result chứa instance Passenger hoặc chi tiết lỗi
     * @throw CoreError nếu bất kỳ đầu vào nào không hợp lệ
     */
    static Result<Passenger> create(const std::string& name, const std::string& contactInfo, const std::string& passport) {
        if (name.empty()) {
            return Failure<Passenger>(CoreError("Passenger name cannot be empty", "INVALID_NAME"));
        }

        auto contactInfoResult = ContactInfo::create(contactInfo);
        if (!contactInfoResult) {
            return Failure<Passenger>(CoreError("Invalid contact info: " + contactInfoResult.error().message, "INVALID_CONTACT_INFO"));
        }

        auto passportResult = PassportNumber::create(passport);
        if (!passportResult) {
            return Failure<Passenger>(CoreError("Invalid passport number: " + passportResult.error().message, "INVALID_PASSPORT"));
        }

        return Success(Passenger(name, *contactInfoResult, *passportResult));
    }

    /**
     * @brief Lấy ID được tạo bởi cơ sở dữ liệu
     * @return ID cơ sở dữ liệu số nguyên
     */
    int getId() const override {
        return _id;
    }

    /**
     * @brief Lấy số hộ chiếu dưới dạng string ID
     * @return Chuỗi số hộ chiếu phục vụ như định danh nghiệp vụ
     */
    std::string getStringId() const override {
        return _passport.toString();
    }

    /**
     * @brief Đặt ID cơ sở dữ liệu
     * @param id ID được tạo bởi cơ sở dữ liệu để gán
     */
    void setId(int id) override {
        _id = id;
    }

    /**
     * @brief Cập nhật tên hành khách
     * @param name Họ tên đầy đủ mới cho hành khách
     */
    void setName(const std::string& name) {
        _name = name;
    }

    /**
     * @brief Lấy biểu diễn chuỗi của hành khách
     * @return Chuỗi định dạng chứa tất cả thuộc tính hành khách
     */
    std::string toString() const override {
        return "Passenger{id=" + std::to_string(_id) + 
               ", name=" + _name + 
               ", contactInfo=" + _contactInfo.toString() + 
               ", passport=" + _passport.toString() + "}";
    }

    /**
     * @brief Kiểm tra tính bằng nhau với thực thể khác
     * @param other Thực thể để so sánh
     * @return true nếu cả hai đều là thực thể Passenger với cùng ID cơ sở dữ liệu
     */
    bool equals(const IEntity& other) const override {
        if (const auto* passenger = dynamic_cast<const Passenger*>(&other)) {
            return _id == passenger->_id;
        }
        return false;
    }

    /**
     * @brief Tạo bản sao sâu của hành khách
     * @return Unique pointer đến instance Passenger mới với giá trị giống hệt
     */
    std::unique_ptr<IEntity> clone() const override {
        auto clone = std::unique_ptr<Passenger>(new Passenger(_name, _contactInfo, _passport));
        clone->_id = _id;
        return clone;
    }

    /**
     * @brief Lấy họ tên đầy đủ của hành khách
     * @return Tham chiếu đến chuỗi tên hành khách
     */
    const Name& getName() const { return _name; }

    /**
     * @brief Lấy thông tin liên lạc của hành khách
     * @return Tham chiếu đến value object ContactInfo
     */
    const ContactInfo& getContactInfo() const { return _contactInfo; }

    /**
     * @brief Lấy số hộ chiếu của hành khách
     * @return Tham chiếu đến value object PassportNumber
     */
    const PassportNumber& getPassport() const { return _passport; }
};

#endif