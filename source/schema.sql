-- Create database
CREATE DATABASE IF NOT EXISTS flight_booking_system;
USE flight_booking_system;

-- Aircraft table
CREATE TABLE aircraft (
    id INT AUTO_INCREMENT PRIMARY KEY,
    serial_number VARCHAR(10),
    model VARCHAR(50) NOT NULL
);

-- Seat class table
CREATE TABLE seat_class (
    id INT AUTO_INCREMENT PRIMARY KEY,
    code CHAR(1) NOT NULL UNIQUE,
    name VARCHAR(20) NOT NULL
);

-- Aircraft seat layout table
CREATE TABLE aircraft_seat_layout (
    id INT AUTO_INCREMENT PRIMARY KEY,
    aircraft_serial VARCHAR(10),
    seat_class_id INT,
    seat_count INT NOT NULL,
    FOREIGN KEY (aircraft_serial) REFERENCES aircraft(serial_number),
    FOREIGN KEY (seat_class_id) REFERENCES seat_class(id),
    UNIQUE KEY unique_aircraft_seat_class (aircraft_serial, seat_class_id)
);

-- Route table
CREATE TABLE route (
    id INT AUTO_INCREMENT PRIMARY KEY,
    departure_code VARCHAR(3) NOT NULL,
    departure_name VARCHAR(100) NOT NULL,
    arrival_code VARCHAR(3) NOT NULL,
    arrival_name VARCHAR(100) NOT NULL,
    UNIQUE KEY unique_route (departure_code, arrival_code)
);

-- Flight table
CREATE TABLE flight (
    id INT AUTO_INCREMENT PRIMARY KEY,
    flight_number VARCHAR(10),
    route_id INT NOT NULL,
    aircraft_serial VARCHAR(10) NOT NULL,
    departure_time DATETIME NOT NULL,
    arrival_time DATETIME NOT NULL,
    status ENUM('SCHEDULED', 'DELAYED', 'BOARDING', 'DEPARTED', 'IN_FLIGHT', 'LANDED', 'CANCELLED') DEFAULT 'SCHEDULED',
    FOREIGN KEY (route_id) REFERENCES route(id),
    FOREIGN KEY (aircraft_serial) REFERENCES aircraft(serial_number)
);

-- Passenger table
CREATE TABLE passenger (
    id INT AUTO_INCREMENT PRIMARY KEY,
    passport_number VARCHAR(20) PRIMARY KEY,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(100) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    address TEXT
);

-- Ticket table
CREATE TABLE ticket (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ticket_number VARCHAR(20),
    flight_number VARCHAR(10) NOT NULL,
    passenger_passport VARCHAR(20) NOT NULL,
    seat_number VARCHAR(10) NOT NULL,
    price DECIMAL(10,2) NOT NULL,
    currency VARCHAR(3) NOT NULL,
    status ENUM('PENDING', 'CONFIRMED', 'CHECKED_IN', 'BOARDED', 'COMPLETED', 'CANCELLED', 'REFUNDED') DEFAULT 'PENDING',
    FOREIGN KEY (flight_number) REFERENCES flight(flight_number),
    FOREIGN KEY (passenger_passport) REFERENCES passenger(passport_number),
    UNIQUE KEY unique_flight_seat (flight_number, seat_number)
);

-- Insert default seat classes
INSERT INTO seat_class (code, name) VALUES
('E', 'ECONOMY'),
('B', 'BUSINESS'),
('F', 'FIRST');

-- Create indexes for better performance
CREATE INDEX idx_flight_dates ON flight(departure_time, arrival_time);
CREATE INDEX idx_ticket_flight ON ticket(flight_number);
CREATE INDEX idx_ticket_passenger ON ticket(passenger_passport);
CREATE INDEX idx_aircraft_seat_layout ON aircraft_seat_layout(aircraft_serial);

-- Add triggers for data validation
DELIMITER //

CREATE TRIGGER before_flight_insert
BEFORE INSERT ON flight
FOR EACH ROW
BEGIN
    IF NEW.departure_time >= NEW.arrival_time THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Arrival time must be after departure time';
    END IF;
END//

CREATE TRIGGER before_ticket_insert
BEFORE INSERT ON ticket
FOR EACH ROW
BEGIN
    DECLARE seat_class_code CHAR(1);
    DECLARE seat_count INT;
    
    -- Get seat class code from seat number
    SET seat_class_code = LEFT(NEW.seat_number, 1);
    
    -- Check if seat class exists in aircraft layout
    SELECT COUNT(*) INTO seat_count
    FROM aircraft_seat_layout acl
    JOIN flight f ON f.aircraft_serial = acl.aircraft_serial
    JOIN seat_class sc ON sc.id = acl.seat_class_id
    WHERE f.flight_number = NEW.flight_number
    AND sc.code = seat_class_code;
    
    IF seat_count = 0 THEN
        SIGNAL SQLSTATE '45000'
        SET MESSAGE_TEXT = 'Invalid seat class for this aircraft';
    END IF;
END//

DELIMITER ; 