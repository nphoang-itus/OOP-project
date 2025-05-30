/**
 * @file Flight.h
 * @brief Thực thể Flight đại diện cho các chuyến bay được lên lịch với quản lý chỗ ngồi
 * @author Nguyễn Phúc Hoàng
 * @version 1.0
 */

#ifndef FLIGHT_H
#define FLIGHT_H

#include "IEntity.h"
#include "Aircraft.h"
#include "../value_objects/flight_number/FlightNumber.h"
#include "../value_objects/route/Route.h"
#include "../value_objects/schedule/Schedule.h"
#include "../value_objects/seat_class_map/SeatClassMap.h"
#include "../value_objects/seat_number/SeatNumber.h"
#include "../value_objects/flight_status/FlightStatus.h"
#include "../exceptions/Result.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>
#include <iomanip>

/**
 * @brief Đại diện cho một chuyến bay được lên lịch với tuyến đường, thời gian, máy bay và tình trạng chỗ ngồi
 *
 * Thực thể Flight là một aggregate trung tâm trong hệ thống quản lý chuyến bay kết hợp
 * định danh chuyến bay, thông tin tuyến đường, chi tiết lịch trình, máy bay được gán,
 * và theo dõi tình trạng chỗ ngồi thời gian thực. Nó quản lý chu kỳ hoàn chỉnh của chuyến bay
 * từ lên lịch đến hoàn thành.
 *
 * Các instance Flight xử lý đặt chỗ, chuyển đổi trạng thái và cung cấp logic nghiệp vụ
 * để xác định các thao tác nào được phép dựa trên trạng thái chuyến bay hiện tại.
 */
class Flight : public IEntity
{
protected:
    FlightNumber _flightNumber;                             ///< Định danh chuyến bay duy nhất (ví dụ: "AA123")
    Route _route;                                           ///< Thông tin sân bay xuất phát và đích
    Schedule _schedule;                                     ///< Thời gian khởi hành và đến nơi
    std::shared_ptr<Aircraft> _aircraft;                    ///< Máy bay được gán cho chuyến bay này
    std::unordered_map<SeatNumber, bool> _seatAvailability; ///< Bản đồ tình trạng chỗ ngồi (true = có sẵn)
    FlightStatus _status;                                   ///< Trạng thái hiện tại của chuyến bay

    /**
     * @brief Tạo số ghế cho một hạng cụ thể dựa trên cấu hình máy bay
     * @param classCode Mã hạng ghế (ví dụ: "F", "B", "E")
     * @param count Số ghế cần tạo cho hạng này
     * @param seatLayout Bố cục chỗ ngồi máy bay để validation
     * @return Vector các đối tượng SeatNumber được tạo
     */
    static std::vector<SeatNumber> generateSeatNumbers(const std::string &classCode, int count, const SeatClassMap &seatLayout)
    {
        std::vector<SeatNumber> seats;
        seats.reserve(count);

        // Determine padding based on count
        int padding = count > 99 ? 3 : 2;

        for (int i = 1; i <= count; i++)
        {
            std::stringstream ss;
            ss << classCode << std::setfill('0') << std::setw(padding) << i;
            auto seatNumberResult = SeatNumber::create(ss.str(), seatLayout);
            if (seatNumberResult)
            {
                seats.push_back(*seatNumberResult);
            }
        }
        return seats;
    }

    /**
     * @brief Khởi tạo bản đồ tình trạng chỗ ngồi dựa trên cấu hình máy bay
     * Tạo entries cho tất cả ghế với tình trạng có sẵn ban đầu được đặt thành true
     */
    void initializeSeats()
    {
        const auto &seatLayout = _aircraft->getSeatLayout();
        for (const auto &[seatClass, count] : seatLayout.getSeatCounts())
        {
            auto seatNumbers = generateSeatNumbers(seatClass.getCode(), count, seatLayout);
            for (const auto &seatNumber : seatNumbers)
            {
                _seatAvailability[seatNumber] = true; // true means available
            }
        }
    }

    /**
     * @brief Constructor được bảo vệ để tạo instance chuyến bay
     * @param flightNumber Định danh chuyến bay duy nhất
     * @param route Thông tin tuyến đường chuyến bay
     * @param schedule Thông tin thời gian chuyến bay
     * @param aircraft Máy bay được gán
     */
    Flight(FlightNumber flightNumber, Route route, Schedule schedule, std::shared_ptr<Aircraft> aircraft)
        : _flightNumber(std::move(flightNumber)), _route(std::move(route)), _schedule(std::move(schedule)), _aircraft(std::move(aircraft)), _status(FlightStatus::SCHEDULED)
    {
        initializeSeats();
    }

public:
    /**
     * @brief Tạo chuyến bay từ các value object đã được validate
     * @param flightNumber Số hiệu chuyến bay đã được validate
     * @param route Thông tin tuyến đường đã được validate
     * @param schedule Thông tin lịch trình đã được validate
     * @param aircraft Máy bay được gán (không được null)
     * @return Result chứa instance Flight hoặc chi tiết lỗi
     * @throw CoreError nếu aircraft là null
     */
    static Result<Flight> create(const FlightNumber &flightNumber, const Route &route,
                                 const Schedule &schedule, std::shared_ptr<Aircraft> aircraft)
    {
        if (!aircraft)
        {
            return Failure<Flight>(CoreError("Aircraft cannot be null", "INVALID_AIRCRAFT"));
        }
        return Success(Flight(flightNumber, route, schedule, aircraft));
    }

    /**
     * @brief Tạo chuyến bay từ đầu vào chuỗi với validation
     * @param flightNumber Số hiệu chuyến bay dưới dạng chuỗi
     * @param route Thông tin tuyến đường dưới dạng chuỗi
     * @param schedule Thông tin lịch trình dưới dạng chuỗi
     * @param aircraft Máy bay được gán (không được null)
     * @return Result chứa instance Flight hoặc chi tiết lỗi
     * @throw CoreError nếu bất kỳ đầu vào nào không hợp lệ hoặc aircraft là null
     */
    static Result<Flight> create(const std::string &flightNumber, const std::string &route,
                                 const std::string &schedule, std::shared_ptr<Aircraft> aircraft)
    {
        auto flightNumberResult = FlightNumber::create(flightNumber);
        if (!flightNumberResult)
        {
            return Failure<Flight>(CoreError("Invalid flight number: " + flightNumberResult.error().message, "INVALID_FLIGHT_NUMBER"));
        }

        auto routeResult = Route::create(route);
        if (!routeResult)
        {
            return Failure<Flight>(CoreError("Invalid route: " + routeResult.error().message, "INVALID_ROUTE"));
        }

        auto scheduleResult = Schedule::create(schedule);
        if (!scheduleResult)
        {
            return Failure<Flight>(CoreError("Invalid schedule: " + scheduleResult.error().message, "INVALID_SCHEDULE"));
        }

        if (!aircraft)
        {
            return Failure<Flight>(CoreError("Aircraft cannot be null", "INVALID_AIRCRAFT"));
        }

        return Success(Flight(*flightNumberResult, *routeResult, *scheduleResult, aircraft));
    }

    /**
     * @brief Cập nhật trạng thái chuyến bay
     * @param status Trạng thái chuyến bay mới để đặt
     */
    void setStatus(FlightStatus status)
    {
        _status = status;
    }

    /**
     * @brief Lấy trạng thái chuyến bay hiện tại
     * @return Giá trị enum FlightStatus hiện tại
     */
    FlightStatus getStatus() const
    {
        return _status;
    }

    /**
     * @brief Lấy trạng thái chuyến bay dưới dạng chuỗi tiếng Anh
     * @return Chuỗi trạng thái có thể đọc được bằng tiếng Anh
     */
    std::string getStatusString() const
    {
        return FlightStatusUtil::toString(_status);
    }

    /**
     * @brief Lấy trạng thái chuyến bay bằng tiếng Việt
     * @return Chuỗi trạng thái có thể đọc được bằng tiếng Việt
     */
    std::string getStatusVietnamese() const
    {
        return FlightStatusUtil::toVietnamese(_status);
    }

    /**
     * @brief Lấy ID được tạo bởi cơ sở dữ liệu
     * @return ID cơ sở dữ liệu số nguyên
     */
    int getId() const override
    {
        return _id;
    }

    /**
     * @brief Lấy số hiệu chuyến bay dưới dạng string ID
     * @return Chuỗi số hiệu chuyến bay phục vụ như định danh nghiệp vụ
     */
    std::string getStringId() const override
    {
        return _flightNumber.toString();
    }

    /**
     * @brief Đặt ID cơ sở dữ liệu
     * @param id ID được tạo bởi cơ sở dữ liệu để gán
     */
    void setId(int id) override
    {
        _id = id;
    }

    /**
     * @brief Cập nhật lịch trình chuyến bay
     * @param schedule Thông tin lịch trình mới
     */
    void setSchedule(const Schedule &schedule)
    {
        _schedule = schedule;
    }

    /**
     * @brief Cập nhật máy bay được gán
     * @param aircraft Phân công máy bay mới
     */
    void setAircraft(std::shared_ptr<Aircraft> aircraft)
    {
        _aircraft = aircraft;
    }

    /**
     * @brief Lấy biểu diễn chuỗi của chuyến bay
     * @return Chuỗi định dạng chứa tất cả thuộc tính chuyến bay
     */
    std::string toString() const override
    {
        return "Flight{id=" + std::to_string(_id) +
               ", flightNumber=" + _flightNumber.toString() +
               ", route=" + _route.toString() +
               ", schedule=" + _schedule.toString() +
               ", aircraft=" + _aircraft->toString() +
               ", status=" + getStatusString() + "}";
    }

    /**
     * @brief Kiểm tra tính bằng nhau với thực thể khác
     * @param other Thực thể để so sánh
     * @return true nếu cả hai đều là thực thể Flight với cùng ID cơ sở dữ liệu
     */
    bool equals(const IEntity &other) const override
    {
        if (const auto *flight = dynamic_cast<const Flight *>(&other))
        {
            return _id == flight->_id;
        }
        return false;
    }

    /**
     * @brief Tạo bản sao sâu của chuyến bay
     * @return Unique pointer đến instance Flight mới với giá trị giống hệt
     */
    std::unique_ptr<IEntity> clone() const override
    {
        auto clone = std::unique_ptr<Flight>(new Flight(_flightNumber, _route, _schedule, _aircraft));
        clone->_id = _id;
        clone->_status = _status;
        clone->_seatAvailability = _seatAvailability;
        return clone;
    }

    /**
     * @brief Lấy số hiệu chuyến bay
     * @return Tham chiếu đến value object FlightNumber
     */
    const FlightNumber &getFlightNumber() const { return _flightNumber; }

    /**
     * @brief Lấy tuyến đường chuyến bay
     * @return Tham chiếu đến value object Route chứa xuất phát và đích
     */
    const Route &getRoute() const { return _route; }

    /**
     * @brief Lấy lịch trình chuyến bay
     * @return Tham chiếu đến value object Schedule chứa thông tin thời gian
     */
    const Schedule &getSchedule() const { return _schedule; }

    /**
     * @brief Lấy máy bay được gán
     * @return Shared pointer đến thực thể Aircraft
     */
    const std::shared_ptr<Aircraft> &getAircraft() const { return _aircraft; }

    /**
     * @brief Lấy bản đồ tình trạng chỗ ngồi hiện tại
     * @return Tham chiếu đến map số ghế đến trạng thái có sẵn
     */
    const std::unordered_map<SeatNumber, bool> &getSeatAvailability() const { return _seatAvailability; }

    /**
     * @brief Kiểm tra xem một ghế cụ thể có sẵn để đặt không
     * @param seatNumberStr Số ghế dưới dạng chuỗi (ví dụ: "1A", "F001")
     * @return true nếu ghế tồn tại và có sẵn, false nếu ngược lại
     */
    bool isSeatAvailable(const std::string &seatNumberStr) const
    {
        if (_status == FlightStatus::CANCELLED)
        {
            return false;
        }
        auto seatNumberResult = SeatNumber::create(seatNumberStr, _aircraft->getSeatLayout());
        if (!seatNumberResult)
        {
            return false;
        }
        const SeatNumber &seatNumber = *seatNumberResult;
        auto it = _seatAvailability.find(seatNumber);
        return it != _seatAvailability.end() && it->second;
    }

    /**
     * @brief Đặt trước một ghế để booking
     * @param seatNumberStr Số ghế dưới dạng chuỗi để đặt trước
     * @return true nếu ghế được đặt trước thành công, false nếu không có sẵn hoặc trạng thái chuyến bay ngăn cản đặt chỗ
     */
    bool reserveSeat(const std::string &seatNumberStr)
    {
        if (_status == FlightStatus::CANCELLED || _status == FlightStatus::DEPARTED ||
            _status == FlightStatus::IN_FLIGHT || _status == FlightStatus::LANDED)
        {
            return false;
        }
        auto seatNumberResult = SeatNumber::create(seatNumberStr, _aircraft->getSeatLayout());
        if (!seatNumberResult)
        {
            return false;
        }
        const SeatNumber &seatNumber = *seatNumberResult;
        auto it = _seatAvailability.find(seatNumber);
        if (it != _seatAvailability.end() && it->second)
        {
            it->second = false;
            return true;
        }
        return false;
    }

    /**
     * @brief Giải phóng một ghế đã được đặt trước
     * @param seatNumberStr Số ghế dưới dạng chuỗi để giải phóng
     * @return true nếu ghế được giải phóng thành công, false nếu chưa được đặt hoặc trạng thái chuyến bay ngăn cản thay đổi
     */
    bool releaseSeat(const std::string &seatNumberStr)
    {
        if (_status == FlightStatus::CANCELLED || _status == FlightStatus::DEPARTED ||
            _status == FlightStatus::IN_FLIGHT || _status == FlightStatus::LANDED)
        {
            return false;
        }
        auto seatNumberResult = SeatNumber::create(seatNumberStr, _aircraft->getSeatLayout());
        if (!seatNumberResult)
        {
            return false;
        }
        const SeatNumber &seatNumber = *seatNumberResult;
        auto it = _seatAvailability.find(seatNumber);
        if (it != _seatAvailability.end() && !it->second)
        {
            it->second = true;
            return true;
        }
        return false;
    }

    /**
     * @brief Khởi tạo tình trạng chỗ ngồi từ dữ liệu cơ sở dữ liệu
     * @param seatAvailability Map số ghế đến trạng thái có sẵn từ cơ sở dữ liệu
     */
    void initializeSeats(const std::map<SeatNumber, bool> &seatAvailability)
    {
        _seatAvailability.clear();
        for (const auto &[seatNumber, isAvailable] : seatAvailability)
        {
            _seatAvailability[seatNumber] = isAvailable;
        }
    }

    void setSeatAvailability(const std::unordered_map<SeatNumber, bool> &seatAvailability)
    {
        _seatAvailability = seatAvailability;
    }
};

#endif