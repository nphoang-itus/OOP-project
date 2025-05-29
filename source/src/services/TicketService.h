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

// Base Strategy Interface (ISP: Small, focused interface)
class ITicketSearchStrategy {
public:
    virtual ~ITicketSearchStrategy() = default;
    virtual Result<std::vector<Ticket>> execute() = 0;
    virtual std::string getStrategyName() const = 0;
};

// Concrete Strategies (Open/Closed Principle: Open for extension)
class PriceRangeSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;
    Price _minPrice;
    Price _maxPrice;
    std::shared_ptr<Logger> _logger;

public:
    PriceRangeSearchStrategy(std::shared_ptr<TicketRepository> repository,
                           const Price& minPrice,
                           const Price& maxPrice,
                           std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

class FlightSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;
    FlightNumber _flightNumber;
    std::shared_ptr<Logger> _logger;

public:
    FlightSearchStrategy(std::shared_ptr<TicketRepository> repository,
                       const FlightNumber& flightNumber,
                       std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

class StatusSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;
    TicketStatus _status;
    std::shared_ptr<Logger> _logger;

public:
    StatusSearchStrategy(std::shared_ptr<TicketRepository> repository,
                       TicketStatus status,
                       std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

class PassengerSearchStrategy : public ITicketSearchStrategy {
private:
    std::shared_ptr<TicketRepository> _repository;
    PassportNumber _passport;
    std::shared_ptr<Logger> _logger;

public:
    PassengerSearchStrategy(std::shared_ptr<TicketRepository> repository,
                          const PassportNumber& passport,
                          std::shared_ptr<Logger> logger = nullptr);

    Result<std::vector<Ticket>> execute() override;
    std::string getStrategyName() const override;
};

// =============================================================================
// FACTORY PATTERN - Open/Closed + Dependency Inversion Principles
// =============================================================================

enum class SearchType {
    PRICE_RANGE,
    FLIGHT,
    STATUS,
    PASSENGER
};

// Factory Interface (Dependency Inversion: Depend on abstraction)
class ITicketSearchStrategyFactory {
public:
    virtual ~ITicketSearchStrategyFactory() = default;
    virtual std::unique_ptr<ITicketSearchStrategy> createStrategy(
        SearchType type, 
        const std::map<std::string, std::string>& params) = 0;
};

// Concrete Factory (Open/Closed: Easy to extend with new strategies)
class TicketSearchStrategyFactory : public ITicketSearchStrategyFactory {
private:
    std::shared_ptr<TicketRepository> _repository;
    std::shared_ptr<Logger> _logger;

public:
    TicketSearchStrategyFactory(std::shared_ptr<TicketRepository> repository,
                              std::shared_ptr<Logger> logger = nullptr);

    std::unique_ptr<ITicketSearchStrategy> createStrategy(
        SearchType type, 
        const std::map<std::string, std::string>& params) override;
};

// =============================================================================
// BUILDER PATTERN - Fluent Interface
// =============================================================================

// Builder for complex searches (Single Responsibility)
class TicketSearchBuilder {
private:
    std::shared_ptr<TicketRepository> _repository;
    std::shared_ptr<Logger> _logger;
    
    // Optional criteria (Composition over inheritance)
    std::optional<FlightNumber> _flightNumber;
    std::optional<Price> _minPrice;
    std::optional<Price> _maxPrice;
    std::optional<TicketStatus> _status;
    std::optional<PassportNumber> _passport;
    std::optional<int> _limit;
    std::optional<std::string> _sortBy;
    bool _sortAscending;

public:
    explicit TicketSearchBuilder(std::shared_ptr<TicketRepository> repository,
                               std::shared_ptr<Logger> logger = nullptr);

    // Fluent interface methods (Method chaining)
    TicketSearchBuilder& withFlight(const FlightNumber& flightNumber);
    TicketSearchBuilder& withPriceRange(const Price& minPrice, const Price& maxPrice);
    TicketSearchBuilder& withStatus(TicketStatus status);
    TicketSearchBuilder& withPassenger(const PassportNumber& passport);
    TicketSearchBuilder& withLimit(int limit);
    TicketSearchBuilder& sortBy(const std::string& field, bool ascending = true);

    // Build and execute
    Result<std::vector<Ticket>> execute();
    TicketSearchBuilder& reset();
};

// =============================================================================
// MAIN SERVICE - Single Responsibility + Dependency Inversion
// =============================================================================

class TicketService {
private:
    // Dependencies (Dependency Inversion: Depend on abstractions)
    std::shared_ptr<TicketRepository> _ticketRepository;
    std::shared_ptr<PassengerRepository> _passengerRepository;
    std::shared_ptr<FlightRepository> _flightRepository;
    std::shared_ptr<AircraftRepository> _aircraftRepository;
    std::shared_ptr<Logger> _logger;
    std::unique_ptr<ITicketSearchStrategyFactory> _searchFactory;

    // Private helper methods (Single Responsibility)
    Result<Ticket> getTicketById(int id);
    Result<bool> existsById(const int& id);
    Result<bool> deleteById(const int& id);

public:
    // Constructor with Dependency Injection
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
    Result<Ticket> getTicket(const TicketNumber& ticketNumber);
    Result<std::vector<Ticket>> getAllTickets();
    Result<bool> ticketExists(const TicketNumber& ticketNumber);
    Result<Ticket> createTicket(const Ticket& ticket);
    Result<Ticket> updateTicket(const Ticket& ticket);
    Result<bool> deleteTicket(const TicketNumber& ticketNumber);

    // =============================================================================
    // BOOKING OPERATIONS (2 methods) - Interface Segregation
    // =============================================================================
    Result<Ticket> bookTicket(const PassportNumber& passport, 
                             const FlightNumber& flightNumber, 
                             const std::string& seatClass,
                             const Price& price);
    Result<bool> cancelTicket(const TicketNumber& ticketNumber, const std::string& reason);

    // =============================================================================
    // STATUS MANAGEMENT (4 methods) - Single Responsibility
    // =============================================================================
    Result<bool> updateTicketStatus(const TicketNumber& ticketNumber, TicketStatus status);
    Result<bool> checkInTicket(const TicketNumber& ticketNumber);
    Result<bool> boardPassenger(const TicketNumber& ticketNumber);
    Result<bool> refundTicket(const TicketNumber& ticketNumber, const std::string& reason);

    // =============================================================================
    // SEARCH OPERATIONS - Strategy Pattern (UI-Friendly) (5 methods)
    // =============================================================================
    
    // Generic strategy-based search (for advanced users)
    Result<std::vector<Ticket>> searchTickets(SearchType type, 
                                             const std::map<std::string, std::string>& params);
    
    // Convenient wrapper methods (UI-friendly, internally use Strategy)
    Result<std::vector<Ticket>> searchByPriceRange(const Price& minPrice, const Price& maxPrice);
    Result<std::vector<Ticket>> searchByFlight(const FlightNumber& flightNumber);
    Result<std::vector<Ticket>> searchByStatus(TicketStatus status);
    Result<std::vector<Ticket>> searchByPassenger(const PassportNumber& passport);

    // Builder access for complex searches
    TicketSearchBuilder createSearchBuilder();

    // =============================================================================
    // BUSINESS VALIDATION (5 methods) - Liskov Substitution
    // =============================================================================
    Result<bool> canCancelTicket(const TicketNumber& ticketNumber);
    Result<bool> canCheckIn(const TicketNumber& ticketNumber);
    Result<bool> canRefund(const TicketNumber& ticketNumber);
    Result<bool> isTicketExpired(const TicketNumber& ticketNumber);
    Result<bool> canBookFlight(const PassportNumber& passport, const FlightNumber& flightNumber);

    // =============================================================================
    // UTILITY METHODS (2 methods) - Interface Segregation
    // =============================================================================
    Result<bool> hasActiveTickets(const PassportNumber& passport);
    Result<int> getTicketCount();
};

#endif // TICKET_SERVICE_H