#include "TicketService.h"
#include "../core/exceptions/Result.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

// =============================================================================
// STRATEGY PATTERN IMPLEMENTATIONS
// =============================================================================

PriceRangeSearchStrategy::PriceRangeSearchStrategy(
    std::shared_ptr<TicketRepository> repository,
    const Price& minPrice,
    const Price& maxPrice,
    std::shared_ptr<Logger> logger)
    : _repository(repository), _minPrice(minPrice), _maxPrice(maxPrice), _logger(logger) {}

Result<std::vector<Ticket>> PriceRangeSearchStrategy::execute() {
    if (_logger) _logger->debug("Executing price range search strategy");
    std::map<std::string, std::string> params;
    params["minPrice"] = std::to_string(_minPrice.getAmount());
    params["maxPrice"] = std::to_string(_maxPrice.getAmount());
    return _repository->findByCriteria(params);
}

std::string PriceRangeSearchStrategy::getStrategyName() const {
    return "PriceRangeSearch";
}

FlightSearchStrategy::FlightSearchStrategy(
    std::shared_ptr<TicketRepository> repository,
    const FlightNumber& flightNumber,
    std::shared_ptr<Logger> logger)
    : _repository(repository), _flightNumber(flightNumber), _logger(logger) {}

Result<std::vector<Ticket>> FlightSearchStrategy::execute() {
    if (_logger) _logger->debug("Executing flight search strategy");
    std::map<std::string, std::string> params;
    params["flightNumber"] = _flightNumber.toString();
    return _repository->findByCriteria(params);
}

std::string FlightSearchStrategy::getStrategyName() const {
    return "FlightSearch";
}

StatusSearchStrategy::StatusSearchStrategy(
    std::shared_ptr<TicketRepository> repository,
    TicketStatus status,
    std::shared_ptr<Logger> logger)
    : _repository(repository), _status(status), _logger(logger) {}

Result<std::vector<Ticket>> StatusSearchStrategy::execute() {
    if (_logger) _logger->debug("Executing status search strategy");
    std::map<std::string, std::string> params;
    params["status"] = TicketStatusUtil::toString(_status);
    return _repository->findByCriteria(params);
}

std::string StatusSearchStrategy::getStrategyName() const {
    return "StatusSearch";
}

PassengerSearchStrategy::PassengerSearchStrategy(
    std::shared_ptr<TicketRepository> repository,
    const PassportNumber& passport,
    std::shared_ptr<Logger> logger)
    : _repository(repository), _passport(passport), _logger(logger) {}

Result<std::vector<Ticket>> PassengerSearchStrategy::execute() {
    if (_logger) _logger->debug("Executing passenger search strategy");
    std::map<std::string, std::string> params;
    params["passport"] = _passport.toString();
    return _repository->findByCriteria(params);
}

std::string PassengerSearchStrategy::getStrategyName() const {
    return "PassengerSearch";
}

// =============================================================================
// FACTORY PATTERN IMPLEMENTATIONS
// =============================================================================

TicketSearchStrategyFactory::TicketSearchStrategyFactory(
    std::shared_ptr<TicketRepository> repository,
    std::shared_ptr<Logger> logger)
    : _repository(repository), _logger(logger) {}

std::unique_ptr<ITicketSearchStrategy> TicketSearchStrategyFactory::createStrategy(
    SearchType type,
    const std::map<std::string, std::string>& params) {
    
    if (_logger) _logger->debug("Creating search strategy of type: " + std::to_string(static_cast<int>(type)));

    switch (type) {
        case SearchType::PRICE_RANGE: {
            auto minPrice = Price::create(params.at("minPrice"));
            auto maxPrice = Price::create(params.at("maxPrice"));
            if (!minPrice || !maxPrice) {
                if (_logger) _logger->error("Invalid price parameters");
                return nullptr;
            }
            return std::make_unique<PriceRangeSearchStrategy>(_repository, *minPrice, *maxPrice, _logger);
        }
        case SearchType::FLIGHT: {
            auto flightNumber = FlightNumber::create(params.at("flightNumber"));
            if (!flightNumber) {
                if (_logger) _logger->error("Invalid flight number parameter");
                return nullptr;
            }
            return std::make_unique<FlightSearchStrategy>(_repository, *flightNumber, _logger);
        }
        case SearchType::STATUS: {
            try {
                auto status = TicketStatusUtil::fromString(params.at("status"));
                return std::make_unique<StatusSearchStrategy>(_repository, status, _logger);
            } catch (const std::exception& e) {
                if (_logger) _logger->error("Invalid status parameter: " + std::string(e.what()));
                return nullptr;
            }
        }
        case SearchType::PASSENGER: {
            auto passport = PassportNumber::create(params.at("passport"));
            if (!passport) {
                if (_logger) _logger->error("Invalid passport parameter");
                return nullptr;
            }
            return std::make_unique<PassengerSearchStrategy>(_repository, *passport, _logger);
        }
        default:
            if (_logger) _logger->error("Unknown search type");
            return nullptr;
    }
}

// =============================================================================
// BUILDER PATTERN IMPLEMENTATIONS
// =============================================================================

TicketSearchBuilder::TicketSearchBuilder(
    std::shared_ptr<TicketRepository> repository,
    std::shared_ptr<Logger> logger)
    : _repository(repository), _logger(logger), _sortAscending(true) {}

TicketSearchBuilder& TicketSearchBuilder::withFlight(const FlightNumber& flightNumber) {
    _flightNumber = flightNumber;
    return *this;
}

TicketSearchBuilder& TicketSearchBuilder::withPriceRange(const Price& minPrice, const Price& maxPrice) {
    _minPrice = minPrice;
    _maxPrice = maxPrice;
    return *this;
}

TicketSearchBuilder& TicketSearchBuilder::withStatus(TicketStatus status) {
    _status = status;
    return *this;
}

TicketSearchBuilder& TicketSearchBuilder::withPassenger(const PassportNumber& passport) {
    _passport = passport;
    return *this;
}

TicketSearchBuilder& TicketSearchBuilder::withLimit(int limit) {
    _limit = limit;
    return *this;
}

TicketSearchBuilder& TicketSearchBuilder::sortBy(const std::string& field, bool ascending) {
    _sortBy = field;
    _sortAscending = ascending;
    return *this;
}

Result<std::vector<Ticket>> TicketSearchBuilder::execute() {
    if (_logger) _logger->debug("Executing complex ticket search");

    // Build search criteria
    std::map<std::string, std::string> params;
    if (_flightNumber) params["flightNumber"] = _flightNumber->toString();
    if (_minPrice) params["minPrice"] = std::to_string(_minPrice->getAmount());
    if (_maxPrice) params["maxPrice"] = std::to_string(_maxPrice->getAmount());
    if (_status) params["status"] = TicketStatusUtil::toString(*_status);
    if (_passport) params["passport"] = _passport->toString();

    // Execute search
    auto result = _repository->findByCriteria(params, _limit, _sortBy, _sortAscending);
    if (!result) {
        if (_logger) _logger->error("Failed to execute search");
        return result;
    }

    return result;
}

TicketSearchBuilder& TicketSearchBuilder::reset() {
    _flightNumber = std::nullopt;
    _minPrice = std::nullopt;
    _maxPrice = std::nullopt;
    _status = std::nullopt;
    _passport = std::nullopt;
    _limit = std::nullopt;
    _sortBy = std::nullopt;
    _sortAscending = true;
    return *this;
}

// =============================================================================
// MAIN SERVICE IMPLEMENTATIONS
// =============================================================================

TicketService::TicketService(
    std::shared_ptr<TicketRepository> ticketRepository,
    std::shared_ptr<PassengerRepository> passengerRepository,
    std::shared_ptr<FlightRepository> flightRepository,
    std::shared_ptr<AircraftRepository> aircraftRepository,
    std::shared_ptr<Logger> logger,
    std::unique_ptr<ITicketSearchStrategyFactory> searchFactory)
    : _ticketRepository(ticketRepository)
    , _passengerRepository(passengerRepository)
    , _flightRepository(flightRepository)
    , _aircraftRepository(aircraftRepository)
    , _logger(logger)
    , _searchFactory(searchFactory ? std::move(searchFactory) 
        : std::make_unique<TicketSearchStrategyFactory>(ticketRepository, logger)) {}

// Private helper methods
Result<Ticket> TicketService::getTicketById(int id) {
    return _ticketRepository->findById(id);
}

Result<bool> TicketService::existsById(const int& id) {
    return _ticketRepository->exists(id);
}

Result<bool> TicketService::deleteById(const int& id) {
    return _ticketRepository->deleteById(id);
}

// Core CRUD operations
Result<Ticket> TicketService::getTicket(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Getting ticket: " + ticketNumber.toString());
    return _ticketRepository->findByTicketNumber(ticketNumber);
}

Result<std::vector<Ticket>> TicketService::getAllTickets() {
    if (_logger) _logger->debug("Getting all tickets");
    return _ticketRepository->findAll();
}

Result<bool> TicketService::ticketExists(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Checking if ticket exists: " + ticketNumber.toString());
    return _ticketRepository->existsTicket(ticketNumber);
}

Result<Ticket> TicketService::createTicket(const Ticket& ticket) {
    if (_logger) _logger->debug("Creating ticket: " + ticket.getTicketNumber().toString());

    // Business rule: Check if ticket number already exists
    auto existsResult = _ticketRepository->existsTicket(ticket.getTicketNumber());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check ticket existence");
        return Failure<Ticket>(CoreError("Failed to check ticket existence", "DB_ERROR"));
    }
    if (existsResult.value()) {
        if (_logger) _logger->error("Ticket number " + ticket.getTicketNumber().toString() + " already exists");
        return Failure<Ticket>(CoreError("Ticket number already exists", "DUPLICATE_TICKET"));
    }

    // Create ticket directly using the input ticket
    return _ticketRepository->create(ticket);
}

Result<Ticket> TicketService::updateTicket(const Ticket& ticket) {
    if (_logger) _logger->debug("Updating ticket: " + ticket.getTicketNumber().toString());

    // Check if ticket exists
    auto existsResult = _ticketRepository->existsTicket(ticket.getTicketNumber());
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check ticket existence");
        return Failure<Ticket>(CoreError("Failed to check ticket existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Ticket " + ticket.getTicketNumber().toString() + " not found");
        return Failure<Ticket>(CoreError("Ticket not found", "NOT_FOUND"));
    }

    // Update ticket
    return _ticketRepository->update(ticket);
}

Result<bool> TicketService::deleteTicket(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Deleting ticket: " + ticketNumber.toString());

    // Check if ticket exists
    auto existsResult = _ticketRepository->existsTicket(ticketNumber);
    if (!existsResult) {
        if (_logger) _logger->error("Failed to check ticket existence");
        return Failure<bool>(CoreError("Failed to check ticket existence", "DB_ERROR"));
    }
    if (!existsResult.value()) {
        if (_logger) _logger->error("Ticket " + ticketNumber.toString() + " not found");
        return Failure<bool>(CoreError("Ticket not found", "NOT_FOUND"));
    }

    // Get ticket to check status
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket details");
        return Failure<bool>(ticketResult.error());
    }

    // Business rule: Check if ticket can be deleted
    auto canDeleteResult = canCancelTicket(ticketNumber);
    if (!canDeleteResult) {
        if (_logger) _logger->error("Failed to check if ticket can be deleted");
        return Failure<bool>(canDeleteResult.error());
    }
    if (!canDeleteResult.value()) {
        if (_logger) _logger->error("Cannot delete ticket that cannot be cancelled");
        return Failure<bool>(CoreError("Cannot delete ticket that cannot be cancelled", "TICKET_CANNOT_BE_DELETED"));
    }

    // Delete ticket
    return _ticketRepository->deleteById(ticketResult.value().getId());
}

// Booking operations
Result<Ticket> TicketService::bookTicket(
    const PassportNumber& passport,
    const FlightNumber& flightNumber,
    const std::string& seatClass,
    const Price& price) {
    
    if (_logger) _logger->debug("Booking ticket for passenger " + passport.toString() + " on flight " + flightNumber.toString());

    // Check if passenger exists
    auto passengerResult = _passengerRepository->findByPassportNumber(passport);
    if (!passengerResult) {
        if (_logger) _logger->error("Failed to get passenger");
        return Failure<Ticket>(passengerResult.error());
    }

    // Check if flight exists
    auto flightResult = _flightRepository->findByFlightNumber(flightNumber);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<Ticket>(flightResult.error());
    }

    // Check if passenger can book flight
    auto canBookResult = canBookFlight(passport, flightNumber);
    if (!canBookResult) {
        if (_logger) _logger->error("Failed to check if passenger can book flight");
        return Failure<Ticket>(canBookResult.error());
    }
    if (!canBookResult.value()) {
        if (_logger) _logger->error("Passenger cannot book flight");
        return Failure<Ticket>(CoreError("Passenger cannot book flight", "CANNOT_BOOK_FLIGHT"));
    }

    // Create ticket
    // Get current ticket count for this flight
    auto flightTicketsResult = _ticketRepository->findByFlightId(flightResult.value().getId());
    if (!flightTicketsResult) {
        if (_logger) _logger->error("Failed to get flight tickets");
        return Failure<Ticket>(flightTicketsResult.error());
    }
    
    // Calculate next ticket number
    int nextTicketNumber = flightTicketsResult.value().size() + 1;
    // Lấy ngày hiện tại theo định dạng YYYYMMDD
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm tm_now;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tm_now, &now_c);
    #else
        localtime_r(&now_c, &tm_now);
    #endif
    std::stringstream date_ss;
    date_ss << std::put_time(&tm_now, "%Y%m%d");
    std::string dateStr = date_ss.str();
    auto ticketNumberResult = TicketNumber::create(
        flightResult.value().getFlightNumber().toString() + // Get airline code (e.g. "VN")
        "-" +
        dateStr + // Ngày hiện tại theo định dạng YYYYMMDD
        "-" +
        std::string(4 - std::to_string(nextTicketNumber).length(), '0') + // Pad with leading zeros
        std::to_string(nextTicketNumber) // Add sequential ticket number
    );
    if (!ticketNumberResult) {
        if (_logger) _logger->error("Failed to create ticket number");
        return Failure<Ticket>(ticketNumberResult.error());
    }

    // Create seat number with proper format and validation
    char classCode = seatClass[0];
    int sequenceNumber = std::stoi(seatClass.substr(1));
    
    // Get seat count for this class
    int maxSeats = 0;
    for (const auto& [seatClass, count] : flightResult.value().getAircraft()->getSeatLayout().getSeatCounts()) {
        if (seatClass.getCode()[0] == classCode) {
            maxSeats = count;
            break;
        }
    }
    
    if (maxSeats == 0) {
        if (_logger) _logger->error("Invalid seat class: " + std::string(1, classCode));
        return Failure<Ticket>(CoreError("Invalid seat class", "INVALID_SEAT_CLASS"));
    }
    
    if (sequenceNumber > maxSeats) {
        if (_logger) _logger->error("Seat number " + std::to_string(sequenceNumber) + " exceeds maximum seats " + std::to_string(maxSeats));
        return Failure<Ticket>(CoreError("Seat number exceeds maximum seats", "INVALID_SEAT_NUMBER"));
    }

    // Format seat number based on max seats
    std::stringstream ss;
    ss << classCode;
    if (maxSeats >= 100) {
        ss << std::setfill('0') << std::setw(3) << sequenceNumber;
    } else {
        ss << std::setfill('0') << std::setw(2) << sequenceNumber;
    }
    
    auto seatNumberResult = SeatNumber::create(ss.str(), flightResult.value().getAircraft()->getSeatLayout());
    if (!seatNumberResult) {
        if (_logger) _logger->error("Failed to create seat number");
        return Failure<Ticket>(seatNumberResult.error());
    }

    // Check if seat is available
    if (!flightResult.value().isSeatAvailable(seatNumberResult.value().toString())) {
        if (_logger) _logger->error("Seat " + seatNumberResult.value().toString() + " is not available");
        return Failure<Ticket>(CoreError("Seat is not available", "SEAT_NOT_AVAILABLE"));
    }

    // Reserve the seat
    if (!flightResult.value().reserveSeat(seatNumberResult.value().toString())) {
        if (_logger) _logger->error("Failed to reserve seat " + seatNumberResult.value().toString());
        return Failure<Ticket>(CoreError("Failed to reserve seat", "RESERVE_SEAT_FAILED"));
    }

    auto ticketResult = Ticket::create(
        ticketNumberResult.value(),
        std::make_shared<Passenger>(passengerResult.value()),
        std::make_shared<Flight>(flightResult.value()),
        seatNumberResult.value(),
        price
    );
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to create ticket");
        return Failure<Ticket>(ticketResult.error());
    }

    ticketResult.value().setStatus(TicketStatus::CONFIRMED);

    // Save ticket
    return _ticketRepository->create(ticketResult.value());
}

Result<bool> TicketService::cancelTicket(const TicketNumber& ticketNumber, const std::string& reason) {
    if (_logger) _logger->debug("Cancelling ticket: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Check if ticket can be cancelled
    auto canCancelResult = canCancelTicket(ticketNumber);
    if (!canCancelResult) {
        if (_logger) _logger->error("Failed to check if ticket can be cancelled");
        return Failure<bool>(canCancelResult.error());
    }
    if (!canCancelResult.value()) {
        if (_logger) _logger->error("Ticket cannot be cancelled");
        return Failure<bool>(CoreError("Ticket cannot be cancelled", "CANNOT_CANCEL_TICKET"));
    }

    // Update ticket status
    auto ticket = ticketResult.value();
    ticket.setStatus(TicketStatus::CANCELLED);

    auto updateResult = _ticketRepository->update(ticket);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update ticket status");
        return Failure<bool>(updateResult.error());
    }

    return Success(true);
}

// Status management
Result<bool> TicketService::updateTicketStatus(const TicketNumber& ticketNumber, TicketStatus status) {
    if (_logger) _logger->debug("Updating ticket status: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Update ticket status
    auto ticket = ticketResult.value();
    ticket.setStatus(status);

    auto updateResult = _ticketRepository->update(ticket);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update ticket status");
        return Failure<bool>(updateResult.error());
    }

    return Success(true);
}

Result<bool> TicketService::checkInTicket(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Checking in ticket: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Check if ticket can be checked in
    auto canCheckInResult = canCheckIn(ticketNumber);
    if (!canCheckInResult) {
        if (_logger) _logger->error("Failed to check if ticket can be checked in");
        return Failure<bool>(canCheckInResult.error());
    }
    if (!canCheckInResult.value()) {
        if (_logger) _logger->error("Ticket cannot be checked in");
        return Failure<bool>(CoreError("Ticket cannot be checked in", "CANNOT_CHECK_IN"));
    }

    // Update ticket status
    auto ticket = ticketResult.value();
    ticket.setStatus(TicketStatus::CHECKED_IN);

    auto updateResult = _ticketRepository->update(ticket);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update ticket status");
        return Failure<bool>(updateResult.error());
    }

    return Success(true);
}

Result<bool> TicketService::boardPassenger(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Boarding passenger with ticket: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Check if ticket is checked in
    auto ticket = ticketResult.value();
    if (ticket.getStatus() != TicketStatus::CHECKED_IN) {
        if (_logger) _logger->error("Ticket is not checked in");
        return Failure<bool>(CoreError("Ticket is not checked in", "NOT_CHECKED_IN"));
    }

    // Update ticket status
    ticket.setStatus(TicketStatus::BOARDED);

    auto updateResult = _ticketRepository->update(ticket);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update ticket status");
        return Failure<bool>(updateResult.error());
    }

    return Success(true);
}

Result<bool> TicketService::refundTicket(const TicketNumber& ticketNumber, const std::string& reason) {
    if (_logger) _logger->debug("Refunding ticket: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Check if ticket can be refunded
    auto canRefundResult = canRefund(ticketNumber);
    if (!canRefundResult) {
        if (_logger) _logger->error("Failed to check if ticket can be refunded");
        return Failure<bool>(canRefundResult.error());
    }
    if (!canRefundResult.value()) {
        if (_logger) _logger->error("Ticket cannot be refunded");
        return Failure<bool>(CoreError("Ticket cannot be refunded", "CANNOT_REFUND_TICKET"));
    }

    // Update ticket status
    auto ticket = ticketResult.value();
    ticket.setStatus(TicketStatus::REFUNDED);

    auto updateResult = _ticketRepository->update(ticket);
    if (!updateResult) {
        if (_logger) _logger->error("Failed to update ticket status");
        return Failure<bool>(updateResult.error());
    }

    return Success(true);
}

// Search operations
Result<std::vector<Ticket>> TicketService::searchTickets(
    SearchType type,
    const std::map<std::string, std::string>& params) {
    
    if (_logger) _logger->debug("Searching tickets with type: " + std::to_string(static_cast<int>(type)));

    auto strategy = _searchFactory->createStrategy(type, params);
    if (!strategy) {
        if (_logger) _logger->error("Failed to create search strategy");
        return Failure<std::vector<Ticket>>(CoreError("Failed to create search strategy", "INVALID_SEARCH_PARAMS"));
    }

    return strategy->execute();
}

Result<std::vector<Ticket>> TicketService::searchByPriceRange(const Price& minPrice, const Price& maxPrice) {
    std::map<std::string, std::string> params;
    params["minPrice"] = minPrice.toString();
    params["maxPrice"] = maxPrice.toString();
    return searchTickets(SearchType::PRICE_RANGE, params);
}

Result<std::vector<Ticket>> TicketService::searchByFlight(const FlightNumber& flightNumber) {
    std::map<std::string, std::string> params;
    params["flightNumber"] = flightNumber.toString();
    return searchTickets(SearchType::FLIGHT, params);
}

Result<std::vector<Ticket>> TicketService::searchByStatus(TicketStatus status) {
    std::map<std::string, std::string> params;
    params["status"] = TicketStatusUtil::toString(status);
    return searchTickets(SearchType::STATUS, params);
}

Result<std::vector<Ticket>> TicketService::searchByPassenger(const PassportNumber& passport) {
    std::map<std::string, std::string> params;
    params["passport"] = passport.toString();
    return searchTickets(SearchType::PASSENGER, params);
}

TicketSearchBuilder TicketService::createSearchBuilder() {
    return TicketSearchBuilder(_ticketRepository, _logger);
}

// Business validation
Result<bool> TicketService::canCancelTicket(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Checking if ticket can be cancelled: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Get flight
    auto flightResult = _flightRepository->findById(ticketResult.value().getFlight()->getId());
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if flight has departed
    auto flight = flightResult.value();
    auto departureTime = flight.getSchedule().getDeparture();
    auto departureTimePoint = std::chrono::system_clock::from_time_t(std::mktime(const_cast<std::tm*>(&departureTime)));
    return Success(departureTimePoint < std::chrono::system_clock::now());
}

Result<bool> TicketService::canCheckIn(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Checking if ticket can be checked in: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Get flight
    auto flightResult = _flightRepository->findById(ticketResult.value().getFlight()->getId());
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if flight has departed
    auto flight = flightResult.value();
    auto departureTime = flight.getSchedule().getDeparture();
    auto departureTimePoint = std::chrono::system_clock::from_time_t(std::mktime(const_cast<std::tm*>(&departureTime)));
    return Success(departureTimePoint < std::chrono::system_clock::now());
}

Result<bool> TicketService::canRefund(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Checking if ticket can be refunded: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Get flight
    auto flightResult = _flightRepository->findById(ticketResult.value().getFlight()->getId());
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if flight has departed
    auto flight = flightResult.value();
    auto departureTime = flight.getSchedule().getDeparture();
    auto departureTimePoint = std::chrono::system_clock::from_time_t(std::mktime(const_cast<std::tm*>(&departureTime)));
    return Success(departureTimePoint < std::chrono::system_clock::now());
}

Result<bool> TicketService::isTicketExpired(const TicketNumber& ticketNumber) {
    if (_logger) _logger->debug("Checking if ticket is expired: " + ticketNumber.toString());

    // Check if ticket exists
    auto ticketResult = _ticketRepository->findByTicketNumber(ticketNumber);
    if (!ticketResult) {
        if (_logger) _logger->error("Failed to get ticket");
        return Failure<bool>(ticketResult.error());
    }

    // Get flight
    auto flightResult = _flightRepository->findById(ticketResult.value().getFlight()->getId());
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if flight has departed
    auto flight = flightResult.value();
    auto departureTime = flight.getSchedule().getDeparture();
    auto departureTimePoint = std::chrono::system_clock::from_time_t(std::mktime(const_cast<std::tm*>(&departureTime)));
    return Success(departureTimePoint < std::chrono::system_clock::now());
}

Result<bool> TicketService::canBookFlight(const PassportNumber& passport, const FlightNumber& flightNumber) {
    if (_logger) _logger->debug("Checking if passenger can book flight: " + passport.toString() + " - " + flightNumber.toString());

    // Check if passenger exists
    auto passengerResult = _passengerRepository->findByPassportNumber(passport);
    if (!passengerResult) {
        if (_logger) _logger->error("Failed to get passenger");
        return Failure<bool>(passengerResult.error());
    }

    // Check if flight exists
    auto flightResult = _flightRepository->findByFlightNumber(flightNumber);
    if (!flightResult) {
        if (_logger) _logger->error("Failed to get flight");
        return Failure<bool>(flightResult.error());
    }

    // Check if flight has departed
    auto flight = flightResult.value();
    auto departureTime = flight.getSchedule().getDeparture();
    auto departureTimePoint = std::chrono::system_clock::from_time_t(std::mktime(const_cast<std::tm*>(&departureTime)));
    return Success(departureTimePoint < std::chrono::system_clock::now());
}

// Utility methods
Result<bool> TicketService::hasActiveTickets(const PassportNumber& passport) {
    if (_logger) _logger->debug("Checking if passenger has active tickets: " + passport.toString());

    // Check if passenger exists
    auto passengerResult = _passengerRepository->findByPassportNumber(passport);
    if (!passengerResult) {
        if (_logger) _logger->error("Failed to get passenger");
        return Failure<bool>(passengerResult.error());
    }

    // Get all tickets for this passenger
    auto ticketsResult = _ticketRepository->findByPassengerId(passengerResult.value().getId());
    if (!ticketsResult) {
        if (_logger) _logger->error("Failed to get passenger tickets");
        return Failure<bool>(ticketsResult.error());
    }

    // Check if any ticket is active
    for (const auto& ticket : ticketsResult.value()) {
        if (ticket.getStatus() != TicketStatus::CANCELLED && 
            ticket.getStatus() != TicketStatus::REFUNDED) {
            return Success(true);
        }
    }

    return Success(false);
}

Result<int> TicketService::getTicketCount() {
    if (_logger) _logger->debug("Getting total ticket count");
    return _ticketRepository->count();
} 