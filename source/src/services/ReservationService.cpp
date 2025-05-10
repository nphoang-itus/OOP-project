/**
 * @file ReservationService.cpp
 * @author Hoang Phuc Nguyen (nphuchoang.itus@gmail.com)
 * @brief Triển khai lớp ReservationService quản lý các chức năng nghiệp vụ liên quan đến đặt vé.
 * @version 0.1
 * @date 2025-05-10
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ReservationService.h"
#include <stdexcept>
#include <random>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <algorithm>

ReservationService::ReservationService(
    std::shared_ptr<ReservationRepository> reservationRepository,
    std::shared_ptr<FlightService> flightService,
    std::shared_ptr<PassengerService> passengerService
) : _reservationRepository(std::move(reservationRepository)),
    _flightService(std::move(flightService)),
    _passengerService(std::move(passengerService)),
    _logger(Logger::getInstance()) {
    
    _logger->debug("Creating ReservationService instance");
    
    if (!_reservationRepository) {
        _logger->error("ReservationRepository is null in ReservationService constructor");
        throw std::invalid_argument("ReservationRepository cannot be null");
    }
    
    if (!_flightService) {
        _logger->error("FlightService is null in ReservationService constructor");
        throw std::invalid_argument("FlightService cannot be null");
    }
    
    if (!_passengerService) {
        _logger->error("PassengerService is null in ReservationService constructor");
        throw std::invalid_argument("PassengerService cannot be null");
    }
}

std::string ReservationService::generateTicketNumber(const std::string& flightNo, const std::string& passportNo) {
    // Lấy timestamp hiện tại
    auto now = std::chrono::system_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count();
    
    // Tạo chuỗi ngẫu nhiên
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 9999);
    int randomNum = distrib(gen);
    
    // Kết hợp để tạo mã vé
    std::ostringstream ticketStream;
    ticketStream << "TK" 
                 << std::setfill('0') << std::setw(4) << (timestamp % 10000)
                 << "-" << flightNo
                 << "-" << std::setfill('0') << std::setw(4) << randomNum;
    
    return ticketStream.str();
}

// bool ReservationService::validateReservation(const Reservation& reservation) {
//     // Kiểm tra mã vé
//     if (reservation.getTicketNo().empty()) {
//         _logger->error("Invalid reservation: Ticket number is empty");
//         return false;
//     }
    
//     // Kiểm tra flight là null
//     if (!reservation.getFlight()) {
//         _logger->error("Invalid reservation: Flight is null");
//         return false;
//     }
    
//     // Kiểm tra passenger là null
//     if (!reservation.getPassenger()) {
//         _logger->error("Invalid reservation: Passenger is null");
//         return false;
//     }
    
//     // Kiểm tra flight có tồn tại và có thể đặt được
//     if (!_flightService->isFlightAvailable(reservation.getFlight()->getNo())) {
//         _logger->error("Invalid reservation: Flight " + reservation.getFlight()->getNo() + " is not available");
//         return false;
//     }
    
//     return true;
// }

std::vector<Reservation> ReservationService::findAll() {
    _logger->debug("ReservationService::findAll - Retrieving all reservations");
    return _reservationRepository->findAll();
}

std::optional<Reservation> ReservationService::findById(const int& id) {
    _logger->debug("ReservationService::findById - Finding reservation with ID: " + std::to_string(id));
    return _reservationRepository->findById(id);
}

bool ReservationService::save(Reservation& reservation) {
    _logger->debug("ReservationService::save - Saving reservation: " + reservation.getTicketNo());
    
    // // Validate reservation data
    // if (!validateReservation(reservation)) {
    //     return false;
    // }
    
    return _reservationRepository->save(reservation);
}

bool ReservationService::remove(const int& id) {
    _logger->debug("ReservationService::remove - Removing reservation with ID: " + std::to_string(id));
    
    // Lấy thông tin đặt vé trước khi xóa
    auto reservation = _reservationRepository->findById(id);
    if (!reservation) {
        _logger->warning("Reservation with ID " + std::to_string(id) + " not found");
        return false;
    }
    
    // Xóa đặt vé
    bool result = _reservationRepository->remove(id);
    
    return result;
}

std::optional<Reservation> ReservationService::findByTicketNo(const std::string& ticketNo) {
    _logger->debug("ReservationService::findByTicketNo - Finding reservation with ticket number: " + ticketNo);
    return _reservationRepository->findByTicketNo(ticketNo);
}

std::vector<Reservation> ReservationService::findByPassenger(const std::string& passportNo) {
    _logger->debug("ReservationService::findByPassenger - Finding reservations for passenger: " + passportNo);
    
    // Kiểm tra xem hành khách có tồn tại không
    auto passenger = _passengerService->findByPassport(passportNo);
    if (!passenger) {
        _logger->warning("Passenger with passport " + passportNo + " not found");
        return {};
    }
    
    // Lấy tất cả đặt vé và lọc theo hành khách
    std::vector<Reservation> allReservations = _reservationRepository->findAll();
    std::vector<Reservation> passengerReservations;
    
    for (const auto& reservation : allReservations) {
        if (reservation.getPassenger()->getPassport() == passportNo) {
            passengerReservations.push_back(reservation);
        }
    }
    
    _logger->info("Found " + std::to_string(passengerReservations.size()) + " reservations for passenger " + passportNo);
    return passengerReservations;
}

std::vector<Reservation> ReservationService::findByFlight(const std::string& flightNo) {
    _logger->debug("ReservationService::findByFlight - Finding reservations for flight: " + flightNo);
    
    // Kiểm tra xem chuyến bay có tồn tại không
    auto flight = _flightService->findByFlightNo(flightNo);
    if (!flight) {
        _logger->warning("Flight " + flightNo + " not found");
        return {};
    }
    
    // Lấy tất cả đặt vé và lọc theo chuyến bay
    std::vector<Reservation> allReservations = _reservationRepository->findAll();
    std::vector<Reservation> flightReservations;
    
    for (const auto& reservation : allReservations) {
        if (reservation.getFlight()->getNo() == flightNo) {
            flightReservations.push_back(reservation);
        }
    }
    
    _logger->info("Found " + std::to_string(flightReservations.size()) + " reservations for flight " + flightNo);
    return flightReservations;
}

bool ReservationService::update(const Reservation& reservation) {
    _logger->debug("ReservationService::update - Updating reservation with ID: " + std::to_string(reservation.getId()));
    
    // Validate reservation data
    // if (!validateReservation(reservation)) {
    //     return false;
    // }
    
    return _reservationRepository->update(reservation);
}

std::optional<Reservation> ReservationService::createReservation(const std::string& flightNo, const Passenger& passenger) {
    _logger->debug("ReservationService::createReservation - Creating reservation for flight " + flightNo);
    
    // Kiểm tra chuyến bay có tồn tại và có thể đặt được không
    if (!_flightService->isFlightAvailable(flightNo)) {
        _logger->error("Cannot create reservation: Flight " + flightNo + " is not available");
        return std::nullopt;
    }
    
    // Lấy thông tin chuyến bay
    auto flight = _flightService->findByFlightNo(flightNo);
    if (!flight) {
        _logger->error("Cannot create reservation: Flight " + flightNo + " not found");
        return std::nullopt;
    }
    
    // Lưu hoặc cập nhật thông tin hành khách
    Passenger savedPassenger = passenger;
    auto existingPassenger = _passengerService->findByPassport(passenger.getPassport());
    
    if (existingPassenger) {
        // Cập nhật thông tin hành khách nếu cần
        savedPassenger = *existingPassenger;
        
        // Cập nhật các thông tin mới
        if (savedPassenger.getName() != passenger.getName() || 
            savedPassenger.getPhone() != passenger.getPhone() || 
            savedPassenger.getAddress() != passenger.getAddress()) {
            
            savedPassenger.setName(passenger.getName());
            savedPassenger.setPhone(passenger.getPhone());
            savedPassenger.setAddress(passenger.getAddress());
            
            if (!_passengerService->update(savedPassenger)) {
                _logger->error("Failed to update passenger information");
                return std::nullopt;
            }
        }
    } else {
        // Lưu hành khách mới
        if (!_passengerService->save(savedPassenger)) {
            _logger->error("Failed to save new passenger");
            return std::nullopt;
        }
    }
    
    // Tạo mã vé
    std::string ticketNo = generateTicketNumber(flightNo, savedPassenger.getPassport());
    
    // Tạo đối tượng đặt vé
    auto flightPtr = std::make_shared<Flight>(*flight);
    auto passengerPtr = std::make_shared<Passenger>(savedPassenger);
    Reservation newReservation(ticketNo, flightPtr, passengerPtr);
    
    // Lưu đặt vé
    if (!save(newReservation)) {
        _logger->error("Failed to save reservation");
        return std::nullopt;
    }
    
    _logger->info("Reservation created successfully with ticket number: " + ticketNo);
    return newReservation;
}

bool ReservationService::cancelReservation(const std::string& ticketNo) {
    _logger->debug("ReservationService::cancelReservation - Cancelling reservation: " + ticketNo);
    
    // Tìm đặt vé
    auto reservation = _reservationRepository->findByTicketNo(ticketNo);
    if (!reservation) {
        _logger->warning("Reservation with ticket number " + ticketNo + " not found");
        return false;
    }
    
    // Xóa đặt vé
    bool result = _reservationRepository->remove(reservation->getId());
    
    if (result) {
        _logger->info("Reservation " + ticketNo + " cancelled successfully");
    } else {
        _logger->error("Failed to cancel reservation " + ticketNo);
    }
    
    return result;
}