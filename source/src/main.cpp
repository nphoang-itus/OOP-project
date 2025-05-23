#include "core/entities/Aircraft.h"
#include "core/exceptions/Result.h"
#include <iostream>

int main() {
    // auto serial = AircraftSerial::create("VN123");
    // if (!serial) {
    //     auto error = Failure<Aircraft>(CoreError("Invalid aircraft serial: " + serial.error().message, "INVALID_SERIAL"));
    //     if (error.error().code == "INVALID_SERIAL") {
    //         std::cout << error.error().message;
    //     }
    // }

    // auto model = AircraftModel("Boeing");
    // auto seatLayoutResult = SeatClassMap::create({
    //     {SeatClassRegistry::getByName("ECONOMY").value(), 100},    // E001-E100
    //     {SeatClassRegistry::getByName("BUSINESS").value(), 20},    // B01-B20
    //     {SeatClassRegistry::getByName("FIRST").value(), 10}        // F01-F10
    // });

    // // Aircraft air = Aircraft::create(serial.value(), model, seatLayoutResult.value()).value();
    // auto air = Aircraft::create("123", "124", "123");
    // if (!air) {
        
    // }


    return 0;
}