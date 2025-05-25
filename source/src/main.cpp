// #include <iostream>
// #include <memory>
// #include "repositories/MockRepository/TicketMockRepository.h"
// #include "core/entities/Ticket.h"
// #include "core/entities/Passenger.h"
// #include "core/entities/Flight.h"
// #include "core/value_objects/ticket_number/TicketNumber.h"
// #include "core/value_objects/seat_number/SeatNumber.h"
// #include "core/value_objects/seat_class_map/SeatClassMap.h"
// #include "core/value_objects/price/Price.h"
// #include "core/value_objects/flight_number/FlightNumber.h"
// #include "core/value_objects/route/Route.h"
// #include "core/value_objects/schedule/Schedule.h"
// #include "core/entities/Aircraft.h"

// int main()
// {
//     auto logger = Logger::getInstance();
//     TicketMockRepository repository(logger);

//     // Tạo Passenger
//     auto passengerResult = Passenger::create("John Doe", "john.doe123@abc.com|0123456789|123 Main St", "VN:1234567");
//     if (!passengerResult)
//     {
//         std::cerr << "Passenger error: " << passengerResult.error().message << std::endl;
//         return 1;
//     }
//     auto passenger = std::make_shared<Passenger>(passengerResult.value());
//     passenger->setId(1);

//     // Tạo Aircraft
//     auto aircraftResult = Aircraft::create("VN123", "A320", "E:100,B:20,F:10");
//     if (!aircraftResult)
//     {
//         std::cerr << "Aircraft error: " << aircraftResult.error().message << std::endl;
//         return 1;
//     }
//     auto aircraft = std::make_shared<Aircraft>(aircraftResult.value());
//     aircraft->setId(1);

//     // Tạo Flight
//     auto flightNumberResult = FlightNumber::create("VN123");
//     auto routeResult = Route::create("Noi Bai International Airport", "HAN", "Tan Son Nhat International Airport", "SGN");
//     std::time_t now = std::time(nullptr);
//     auto departure = *std::localtime(&now);
//     auto arrival = departure;
//     arrival.tm_hour += 2;
//     auto scheduleResult = Schedule::create(departure, arrival);
//     auto flightResult = Flight::create(flightNumberResult.value(), routeResult.value(), scheduleResult.value(), aircraft);
//     if (!flightResult)
//     {
//         std::cerr << "Flight error: " << flightResult.error().message << std::endl;
//         return 1;
//     }
//     auto flight = std::make_shared<Flight>(flightResult.value());
//     flight->setId(1);

//     // Tạo TicketNumber, SeatClassMap, SeatNumber, Price
//     auto ticketNumberResult = TicketNumber::create("AB12-20230525-0001");
//     auto seatClassMapResult = SeatClassMap::create("E:100,B:20,F:10");
//     auto seatNumberResult = SeatNumber::create("E001", seatClassMapResult.value());
//     auto priceResult = Price::create(100.0, "USD");

//     if (!ticketNumberResult || !seatNumberResult || !priceResult)
//     {
//         std::cerr << "Ticket value object error!" << std::endl;
//         return 1;
//     }

//     // Tạo Ticket
//     auto ticketResult = Ticket::create(ticketNumberResult.value(), passenger, flight, seatNumberResult.value(), priceResult.value());
//     if (!ticketResult)
//     {
//         std::cerr << "Ticket error: " << ticketResult.error().message << std::endl;
//         return 1;
//     }

//     // Thêm ticket vào repository
//     auto createResult = repository.create(ticketResult.value());
//     if (!createResult)
//     {
//         std::cerr << "Create ticket error: " << createResult.error().message << std::endl;
//         return 1;
//     }
//     std::cout << "Created ticket: " << createResult.value().toString() << std::endl;

//     // Tìm ticket theo id
//     auto findResult = repository.findById(createResult.value().getId());
//     if (findResult)
//     {
//         std::cout << "Found by id: " << findResult.value().toString() << std::endl;
//     }

//     // Đếm số lượng ticket
//     auto countResult = repository.count();
//     std::cout << "Ticket count: " << countResult.value() << std::endl;

//     // Xóa ticket
//     auto deleteResult = repository.deleteById(createResult.value().getId());
//     std::cout << "Delete ticket: " << (deleteResult.value() ? "Success" : "Fail") << std::endl;

//     // Đếm lại
//     countResult = repository.count();
//     std::cout << "Ticket count after delete: " << countResult.value() << std::endl;

//     return 0;
// }

#include <gtest/gtest.h>

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}