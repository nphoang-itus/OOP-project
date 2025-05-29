/**
 * @file TicketService.h
 * @brief Dịch vụ quản lý vé máy bay với các pattern thiết kế Strategy, Factory và Builder
 * @author OOP Project Team
 * @date 2024-2025
 */

#ifndef TICKET_SERVICE_H
#define TICKET_SERVICE_H

#include "../core/entities/Ticket.h"
#include "../repositories/MySQLRepository/TicketRepository.h"
#include "../repositories/MySQLRepository/PassengerRepository.h"
#include "../repositories/MySQLRepository/FlightRepository.h"
#include "../repositories/MySQLRepository/AircraftRepository.h"
#include "../core/exceptions/Result.h"
#include "../utils/Logger.h"
#include <memory>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <chrono>

// =============================================================================
// STRATEGY PATTERN - Interface Segregation Principle
// =============================================================================

/**
 * @class ITicketSearchStrategy
 * @brief Interface cho Strategy Pattern - định nghĩa các chiến lược tìm kiếm vé
 * 
 * Interface này tuân theo nguyên tắc Interface Segregation Principle,
 * cung cấp một giao diện nhỏ và tập trung cho các chiến lược tìm kiếm.
 */
class ITicketSearchStrategy {
public:
    virtual ~ITicketSearchStrategy() = default;
    
    /**
     * @brief Thực thi chiến lược tìm kiếm
     * @return Result<std::vector<Ticket>> Danh sách vé tìm được hoặc lỗi
     */
    virtual Result<std::vector<Ticket>> execute() = 0;
    
    /**
     * @brief Lấy tên của chiến lược
     * @return std::string Tên chiến lược
     */
    virtual std::string getStrategyName() const = 0;
};

/**
 * @class PriceRangeSearchStrategy
 * @brief Chiến lược tìm kiếm vé theo khoảng giá
 */
class PriceRangeSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;  ///< Repository để truy cập dữ liệu vé
    Price _minPrice;                                ///< Giá tối thiểu
    Price _maxPrice;                                ///< Giá tối đa
    std::shared_ptr<Logger> _logger;                ///< Logger để ghi log

public:
    /**
     * @brief Constructor khởi tạo chiến lược tìm kiếm theo khoảng giá
     * @param repository Repository để truy cập dữ liệu vé
     * @param minPrice Giá tối thiểu
     * @param maxPrice Giá tối đa
     * @param logger Logger để ghi log (tùy chọn)
     */
    PriceRangeSearchStrategy(std::shared_ptr<TicketRepository> repository,
                           const Price& minPrice,
                           const Price& maxPrice,
                           std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

/**
 * @class FlightSearchStrategy
 * @brief Chiến lược tìm kiếm vé theo số hiệu chuyến bay
 */
class FlightSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;  ///< Repository để truy cập dữ liệu vé
    FlightNumber _flightNumber;                     ///< Số hiệu chuyến bay
    std::shared_ptr<Logger> _logger;                ///< Logger để ghi log

public:
    /**
     * @brief Constructor khởi tạo chiến lược tìm kiếm theo chuyến bay
     * @param repository Repository để truy cập dữ liệu vé
     * @param flightNumber Số hiệu chuyến bay
     * @param logger Logger để ghi log (tùy chọn)
     */
    FlightSearchStrategy(std::shared_ptr<TicketRepository> repository,
                       const FlightNumber& flightNumber,
                       std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

/**
 * @class StatusSearchStrategy
 * @brief Chiến lược tìm kiếm vé theo trạng thái
 */
class StatusSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;  ///< Repository để truy cập dữ liệu vé
    TicketStatus _status;                           ///< Trạng thái vé
    std::shared_ptr<Logger> _logger;                ///< Logger để ghi log

public:
    /**
     * @brief Constructor khởi tạo chiến lược tìm kiếm theo trạng thái
     * @param repository Repository để truy cập dữ liệu vé
     * @param status Trạng thái vé
     * @param logger Logger để ghi log (tùy chọn)
     */
    StatusSearchStrategy(std::shared_ptr<TicketRepository> repository,
                       TicketStatus status,
                       std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

/**
 * @class PassengerSearchStrategy
 * @brief Chiến lược tìm kiếm vé theo hành khách
 */
class PassengerSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;  ///< Repository để truy cập dữ liệu vé
    PassportNumber _passport;                       ///< Số hộ chiếu hành khách
    std::shared_ptr<Logger> _logger;                ///< Logger để ghi log

public:
    /**
     * @brief Constructor khởi tạo chiến lược tìm kiếm theo hành khách
     * @param repository Repository để truy cập dữ liệu vé
     * @param passport Số hộ chiếu hành khách
     * @param logger Logger để ghi log (tùy chọn)
     */
    PassengerSearchStrategy(std::shared_ptr<TicketRepository> repository,
                          const PassportNumber& passport,
                          std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

// =============================================================================
// FACTORY PATTERN - Open/Closed + Dependency Inversion Principles
// =============================================================================

/**
 * @enum SearchType
 * @brief Enum định nghĩa các loại tìm kiếm có sẵn
 */
enum class SearchType {
    PRICE_RANGE,    ///< Tìm kiếm theo khoảng giá
    FLIGHT,         ///< Tìm kiếm theo chuyến bay
    STATUS,         ///< Tìm kiếm theo trạng thái
    PASSENGER       ///< Tìm kiếm theo hành khách
};

/**
 * @class ITicketSearchStrategyFactory
 * @brief Interface cho Factory Pattern - tạo các chiến lược tìm kiếm
 */
class ITicketSearchStrategyFactory {
public:
    virtual ~ITicketSearchStrategyFactory() = default;
    
    /**
     * @brief Tạo chiến lược tìm kiếm theo loại và tham số
     * @param type Loại tìm kiếm
     * @param params Map chứa các tham số tìm kiếm
     * @return std::unique_ptr<ITicketSearchStrategy> Chiến lược tìm kiếm được tạo
     */
    virtual std::unique_ptr<ITicketSearchStrategy> createStrategy(
        SearchType type, 
        const std::map<std::string, std::string>& params) = 0;
};

/**
 * @class TicketSearchStrategyFactory
 * @brief Factory cụ thể để tạo các chiến lược tìm kiếm vé
 */
class TicketSearchStrategyFactory : public ITicketSearchStrategyFactory {
private:
    std::shared_ptr<TicketRepository> _repository;  ///< Repository để truy cập dữ liệu vé
    std::shared_ptr<Logger> _logger;                ///< Logger để ghi log

public:
    /**
     * @brief Constructor khởi tạo factory
     * @param repository Repository để truy cập dữ liệu vé
     * @param logger Logger để ghi log (tùy chọn)
     */
    TicketSearchStrategyFactory(std::shared_ptr<TicketRepository> repository,
                              std::shared_ptr<Logger> logger = nullptr);

    std::unique_ptr<ITicketSearchStrategy> createStrategy(
        SearchType type, 
        const std::map<std::string, std::string>& params) override;
};

// =============================================================================
// BUILDER PATTERN - Fluent Interface
// =============================================================================

/**
 * @class TicketSearchBuilder
 * @brief Builder cho việc tạo các tìm kiếm phức tạp với Fluent Interface
 */
class TicketSearchBuilder {
private:
    std::shared_ptr<TicketRepository> _repository;  ///< Repository để truy cập dữ liệu vé
    std::shared_ptr<Logger> _logger;                ///< Logger để ghi log
    
    // Optional criteria (Composition over inheritance)
    std::optional<FlightNumber> _flightNumber;      ///< Số hiệu chuyến bay (tùy chọn)
    std::optional<Price> _minPrice;                 ///< Giá tối thiểu (tùy chọn)
    std::optional<Price> _maxPrice;                 ///< Giá tối đa (tùy chọn)
    std::optional<TicketStatus> _status;            ///< Trạng thái vé (tùy chọn)
    std::optional<PassportNumber> _passport;        ///< Số hộ chiếu (tùy chọn)
    std::optional<int> _limit;                      ///< Giới hạn số kết quả (tùy chọn)
    std::optional<std::string> _sortBy;             ///< Trường sắp xếp (tùy chọn)
    bool _sortAscending;                            ///< Hướng sắp xếp

public:
    /**
     * @brief Constructor khởi tạo builder
     * @param repository Repository để truy cập dữ liệu vé
     * @param logger Logger để ghi log (tùy chọn)
     */
    explicit TicketSearchBuilder(std::shared_ptr<TicketRepository> repository,
                               std::shared_ptr<Logger> logger = nullptr);

    // Fluent interface methods (Method chaining)
    /**
     * @brief Thêm điều kiện tìm kiếm theo chuyến bay
     * @param flightNumber Số hiệu chuyến bay
     * @return TicketSearchBuilder& Tham chiếu để hỗ trợ method chaining
     */
    TicketSearchBuilder& withFlight(const FlightNumber& flightNumber);
    
    /**
     * @brief Thêm điều kiện tìm kiếm theo khoảng giá
     * @param minPrice Giá tối thiểu
     * @param maxPrice Giá tối đa
     * @return TicketSearchBuilder& Tham chiếu để hỗ trợ method chaining
     */
    TicketSearchBuilder& withPriceRange(const Price& minPrice, const Price& maxPrice);
    
    /**
     * @brief Thêm điều kiện tìm kiếm theo trạng thái
     * @param status Trạng thái vé
     * @return TicketSearchBuilder& Tham chiếu để hỗ trợ method chaining
     */
    TicketSearchBuilder& withStatus(TicketStatus status);
    
    /**
     * @brief Thêm điều kiện tìm kiếm theo hành khách
     * @param passport Số hộ chiếu hành khách
     * @return TicketSearchBuilder& Tham chiếu để hỗ trợ method chaining
     */
    TicketSearchBuilder& withPassenger(const PassportNumber& passport);
    
    /**
     * @brief Thêm giới hạn số kết quả
     * @param limit Số lượng kết quả tối đa
     * @return TicketSearchBuilder& Tham chiếu để hỗ trợ method chaining
     */
    TicketSearchBuilder& withLimit(int limit);
    
    /**
     * @brief Thêm điều kiện sắp xếp
     * @param field Trường để sắp xếp
     * @param ascending true nếu sắp xếp tăng dần, false nếu giảm dần
     * @return TicketSearchBuilder& Tham chiếu để hỗ trợ method chaining
     */
    TicketSearchBuilder& sortBy(const std::string& field, bool ascending = true);

    // Build and execute
    /**
     * @brief Thực thi tìm kiếm với các điều kiện đã thiết lập
     * @return Result<std::vector<Ticket>> Danh sách vé tìm được hoặc lỗi
     */
    Result<std::vector<Ticket>> execute();
    
    /**
     * @brief Reset tất cả điều kiện tìm kiếm
     * @return TicketSearchBuilder& Tham chiếu để hỗ trợ method chaining
     */
    TicketSearchBuilder& reset();
};

// =============================================================================
// MAIN SERVICE - Single Responsibility + Dependency Inversion
// =============================================================================

/**
 * @class TicketService
 * @brief Lớp dịch vụ chính chịu trách nhiệm quản lý các thao tác liên quan đến vé máy bay
 * 
 * Lớp này áp dụng các nguyên tắc SOLID và các pattern thiết kế để cung cấp
 * một interface đầy đủ cho việc quản lý vé, đặt chỗ, tìm kiếm và xác thực nghiệp vụ.
 */
class TicketService {
private:
    // Dependencies (Dependency Inversion: Depend on abstractions)
    std::shared_ptr<TicketRepository> _ticketRepository;        ///< Repository để truy cập dữ liệu vé
    std::shared_ptr<PassengerRepository> _passengerRepository;  ///< Repository để truy cập dữ liệu hành khách
    std::shared_ptr<FlightRepository> _flightRepository;        ///< Repository để truy cập dữ liệu chuyến bay
    std::shared_ptr<AircraftRepository> _aircraftRepository;    ///< Repository để truy cập dữ liệu máy bay
    std::shared_ptr<Logger> _logger;                            ///< Logger để ghi log hệ thống
    std::unique_ptr<ITicketSearchStrategyFactory> _searchFactory; ///< Factory để tạo chiến lược tìm kiếm

    /**
     * @brief Lấy thông tin vé theo ID
     * @param id ID của vé
     * @return Result<Ticket> Kết quả chứa thông tin vé hoặc lỗi
     */
    Result<Ticket> getTicketById(int id);
    
    /**
     * @brief Kiểm tra vé có tồn tại theo ID
     * @param id ID của vé cần kiểm tra
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> existsById(const int& id);
    
    /**
     * @brief Xóa vé theo ID
     * @param id ID của vé cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deleteById(const int& id);

public:
    /**
     * @brief Constructor khởi tạo TicketService với các dependency
     * @param ticketRepository Repository quản lý dữ liệu vé
     * @param passengerRepository Repository quản lý dữ liệu hành khách
     * @param flightRepository Repository quản lý dữ liệu chuyến bay
     * @param aircraftRepository Repository quản lý dữ liệu máy bay
     * @param logger Logger để ghi log (tùy chọn)
     * @param searchFactory Factory để tạo chiến lược tìm kiếm (tùy chọn)
     */
    TicketService(
        std::shared_ptr<TicketRepository> ticketRepository,
        std::shared_ptr<PassengerRepository> passengerRepository,
        std::shared_ptr<FlightRepository> flightRepository,
        std::shared_ptr<AircraftRepository> aircraftRepository,
        std::shared_ptr<Logger> logger = nullptr,
        std::unique_ptr<ITicketSearchStrategyFactory> searchFactory = nullptr
    );

    // =============================================================================
    // CORE CRUD OPERATIONS (6 methods) - Single Responsibility
    // =============================================================================
    /**
     * @brief Lấy thông tin vé theo số vé
     * @param ticketNumber Số vé
     * @return Result<Ticket> Kết quả chứa thông tin vé hoặc lỗi
     */
    Result<Ticket> getTicket(const TicketNumber& ticketNumber);
    
    /**
     * @brief Lấy danh sách tất cả vé
     * @return Result<std::vector<Ticket>> Danh sách tất cả vé hoặc lỗi
     */
    Result<std::vector<Ticket>> getAllTickets();
    
    /**
     * @brief Kiểm tra vé có tồn tại theo số vé
     * @param ticketNumber Số vé
     * @return Result<bool> true nếu tồn tại, false nếu không
     */
    Result<bool> ticketExists(const TicketNumber& ticketNumber);
    
    /**
     * @brief Tạo mới một vé
     * @param ticket Đối tượng vé cần tạo
     * @return Result<Ticket> Vé đã được tạo hoặc lỗi
     */
    Result<Ticket> createTicket(const Ticket& ticket);
    
    /**
     * @brief Cập nhật thông tin vé
     * @param ticket Đối tượng vé với thông tin mới
     * @return Result<Ticket> Vé đã được cập nhật hoặc lỗi
     */
    Result<Ticket> updateTicket(const Ticket& ticket);
    
    /**
     * @brief Xóa vé theo số vé
     * @param ticketNumber Số vé cần xóa
     * @return Result<bool> true nếu xóa thành công, false nếu thất bại
     */
    Result<bool> deleteTicket(const TicketNumber& ticketNumber);

    // =============================================================================
    // BOOKING OPERATIONS (2 methods) - Interface Segregation
    // =============================================================================
    /**
     * @brief Đặt vé cho hành khách
     * @param passport Số hộ chiếu hành khách
     * @param flightNumber Số hiệu chuyến bay
     * @param seatClass Hạng ghế
     * @param price Giá vé
     * @return Result<Ticket> Vé đã được đặt hoặc lỗi
     */
    Result<Ticket> bookTicket(const PassportNumber& passport, 
                             const FlightNumber& flightNumber, 
                             const std::string& seatClass,
                             const Price& price);
    
    /**
     * @brief Hủy vé
     * @param ticketNumber Số vé cần hủy
     * @param reason Lý do hủy
     * @return Result<bool> true nếu hủy thành công, false nếu thất bại
     */
    Result<bool> cancelTicket(const TicketNumber& ticketNumber, const std::string& reason);

    // =============================================================================
    // STATUS MANAGEMENT (4 methods) - Single Responsibility
    // =============================================================================
    /**
     * @brief Cập nhật trạng thái vé
     * @param ticketNumber Số vé
     * @param status Trạng thái mới
     * @return Result<bool> true nếu cập nhật thành công, false nếu thất bại
     */
    Result<bool> updateTicketStatus(const TicketNumber& ticketNumber, TicketStatus status);
    
    /**
     * @brief Check-in vé
     * @param ticketNumber Số vé cần check-in
     * @return Result<bool> true nếu check-in thành công, false nếu thất bại
     */
    Result<bool> checkInTicket(const TicketNumber& ticketNumber);
    
    /**
     * @brief Lên máy bay cho hành khách
     * @param ticketNumber Số vé
     * @return Result<bool> true nếu lên máy bay thành công, false nếu thất bại
     */
    Result<bool> boardPassenger(const TicketNumber& ticketNumber);
    
    /**
     * @brief Hoàn tiền vé
     * @param ticketNumber Số vé cần hoàn tiền
     * @param reason Lý do hoàn tiền
     * @return Result<bool> true nếu hoàn tiền thành công, false nếu thất bại
     */
    Result<bool> refundTicket(const TicketNumber& ticketNumber, const std::string& reason);

    // =============================================================================
    // SEARCH OPERATIONS - Strategy Pattern (UI-Friendly) (5 methods)
    // =============================================================================
    
    /**
     * @brief Tìm kiếm vé sử dụng Strategy Pattern
     * @param type Loại tìm kiếm
     * @param params Map chứa các tham số tìm kiếm
     * @return Result<std::vector<Ticket>> Danh sách vé tìm được hoặc lỗi
     */
    Result<std::vector<Ticket>> searchTickets(SearchType type, 
                                             const std::map<std::string, std::string>& params);
    
    /**
     * @brief Tìm kiếm vé theo khoảng giá
     * @param minPrice Giá tối thiểu
     * @param maxPrice Giá tối đa
     * @return Result<std::vector<Ticket>> Danh sách vé tìm được hoặc lỗi
     */
    Result<std::vector<Ticket>> searchByPriceRange(const Price& minPrice, const Price& maxPrice);
    
    /**
     * @brief Tìm kiếm vé theo chuyến bay
     * @param flightNumber Số hiệu chuyến bay
     * @return Result<std::vector<Ticket>> Danh sách vé tìm được hoặc lỗi
     */
    Result<std::vector<Ticket>> searchByFlight(const FlightNumber& flightNumber);
    
    /**
     * @brief Tìm kiếm vé theo trạng thái
     * @param status Trạng thái vé
     * @return Result<std::vector<Ticket>> Danh sách vé tìm được hoặc lỗi
     */
    Result<std::vector<Ticket>> searchByStatus(TicketStatus status);
    
    /**
     * @brief Tìm kiếm vé theo hành khách
     * @param passport Số hộ chiếu hành khách
     * @return Result<std::vector<Ticket>> Danh sách vé tìm được hoặc lỗi
     */
    Result<std::vector<Ticket>> searchByPassenger(const PassportNumber& passport);

    /**
     * @brief Tạo Builder cho tìm kiếm phức tạp
     * @return TicketSearchBuilder Builder để xây dựng tìm kiếm phức tạp
     */
    TicketSearchBuilder createSearchBuilder();

    // =============================================================================
    // BUSINESS VALIDATION (5 methods) - Liskov Substitution
    // =============================================================================
    /**
     * @brief Kiểm tra có thể hủy vé hay không
     * @param ticketNumber Số vé
     * @return Result<bool> true nếu có thể hủy, false nếu không thể
     */
    Result<bool> canCancelTicket(const TicketNumber& ticketNumber);
    
    /**
     * @brief Kiểm tra có thể check-in hay không
     * @param ticketNumber Số vé
     * @return Result<bool> true nếu có thể check-in, false nếu không thể
     */
    Result<bool> canCheckIn(const TicketNumber& ticketNumber);
    
    /**
     * @brief Kiểm tra có thể hoàn tiền hay không
     * @param ticketNumber Số vé
     * @return Result<bool> true nếu có thể hoàn tiền, false nếu không thể
     */
    Result<bool> canRefund(const TicketNumber& ticketNumber);
    
    /**
     * @brief Kiểm tra vé đã hết hạn chưa
     * @param ticketNumber Số vé
     * @return Result<bool> true nếu đã hết hạn, false nếu còn hiệu lực
     */
    Result<bool> isTicketExpired(const TicketNumber& ticketNumber);
    
    /**
     * @brief Kiểm tra có thể đặt vé cho chuyến bay hay không
     * @param passport Số hộ chiếu hành khách
     * @param flightNumber Số hiệu chuyến bay
     * @return Result<bool> true nếu có thể đặt, false nếu không thể
     */
    Result<bool> canBookFlight(const PassportNumber& passport, const FlightNumber& flightNumber);

    // =============================================================================
    // UTILITY METHODS (2 methods) - Interface Segregation
    // =============================================================================
    /**
     * @brief Kiểm tra hành khách có vé đang hoạt động
     * @param passport Số hộ chiếu hành khách
     * @return Result<bool> true nếu có vé đang hoạt động, false nếu không
     */
    Result<bool> hasActiveTickets(const PassportNumber& passport);
    
    /**
     * @brief Lấy tổng số vé trong hệ thống
     * @return Result<int> Tổng số vé hoặc lỗi
     */
    Result<int> getTicketCount();
};

#endif // TICKET_SERVICE_H