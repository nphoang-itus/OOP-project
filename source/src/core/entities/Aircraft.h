#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "IEntity.h"
#include "../value_objects/aircraft_serial/AircraftSerial.h"
#include "../value_objects/seat_class_map/SeatClassMap.h"
#include "../exceptions/Result.h"
#include <memory>

using AircraftModel = std::string;

class Aircraft : public IEntity {
protected:
    AircraftSerial _serial;
    AircraftModel _model;
    SeatClassMap _seatLayout;

    Aircraft(AircraftSerial serial, AircraftModel model, SeatClassMap seatLayout)
        : _serial(std::move(serial)), _model(std::move(model)), _seatLayout(std::move(seatLayout)) {}

public:
    // Factory methods
    static Result<Aircraft> create(const AircraftSerial& serial, const AircraftModel& model, const SeatClassMap& seatLayout) {
        if (model.empty()) {
            return Failure<Aircraft>(CoreError("Aircraft model cannot be empty", "INVALID_MODEL"));
        }
        return Success(Aircraft(serial, model, seatLayout));
    }

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

    // Getters
    int getId() const override { return _id; }
    std::string getStringId() const override { return _serial.toString(); }
    const AircraftSerial& getSerial() const { return _serial; }
    const AircraftModel& getModel() const { return _model; }
    const SeatClassMap& getSeatLayout() const { return _seatLayout; }

    // Setters
    void setId(int id) override { _id = id; }
    void setModel(const std::string& model) { _model = model; }
    void setSeatLayout(const SeatClassMap& seatLayout) { _seatLayout = seatLayout; }

    // Utility methods
    std::string toString() const override {
        return "Aircraft{id=" + std::to_string(_id) + 
               ", serial=" + _serial.toString() + 
               ", model=" + _model + 
               ", seatLayout=" + _seatLayout.toString() + "}";
    }

    bool equals(const IEntity& other) const override {
        if (const auto* aircraft = dynamic_cast<const Aircraft*>(&other)) {
            return _id == aircraft->_id;
        }
        return false;
    }

    std::unique_ptr<IEntity> clone() const override {
        auto clone = std::unique_ptr<Aircraft>(new Aircraft(_serial, _model, _seatLayout));
        clone->_id = _id;
        return clone;
    }

    // Business logic methods
    bool hasSeatClass(const std::string& seatClassCode) const {
        return _seatLayout.hasSeatClass(seatClassCode);
    }

    int getSeatCount(const std::string& seatClassCode) const {
        return _seatLayout.getSeatCount(seatClassCode);
    }

    bool isValidSeatNumber(const std::string& seatNumber) const {
        return _seatLayout.isValidSeatNumber(seatNumber);
    }
};

#endif