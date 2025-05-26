CREATE DATABASE IF NOT EXISTS airlines_db;
USE airlines_db;

-- Grant necessary privileges for trigger creation
GRANT ALL PRIVILEGES ON airlines_db.* TO 'nphoang'@'%';
SET GLOBAL log_bin_trust_function_creators = 1;
FLUSH PRIVILEGES;

-- Aircraft table
CREATE TABLE aircraft (
    id INT AUTO_INCREMENT PRIMARY KEY,
    serial_number VARCHAR(10) UNIQUE,
    model VARCHAR(50) NOT NULL,
    economy_seats INT NOT NULL DEFAULT 0 CHECK (economy_seats >= 0),
    business_seats INT NOT NULL DEFAULT 0 CHECK (business_seats >= 0),
    first_seats INT NOT NULL DEFAULT 0 CHECK (first_seats >= 0)
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
    aircraft_id INT,
    seat_class_code CHAR(1),
    seat_count INT NOT NULL,
    FOREIGN KEY (aircraft_id) REFERENCES aircraft(id),
    FOREIGN KEY (seat_class_code) REFERENCES seat_class(code),
    UNIQUE KEY unique_aircraft_seat_class (aircraft_id, seat_class_code)
);

-- Flight table (merged with route)
CREATE TABLE flight (
    id INT AUTO_INCREMENT PRIMARY KEY,
    flight_number VARCHAR(6) NOT NULL,
    departure_code VARCHAR(3) NOT NULL,
    departure_name VARCHAR(100) NOT NULL,
    arrival_code VARCHAR(3) NOT NULL,
    arrival_name VARCHAR(100) NOT NULL,
    aircraft_id INT NOT NULL,
    departure_time DATETIME NOT NULL,
    arrival_time DATETIME NOT NULL,
    status ENUM('SCHEDULED', 'DELAYED', 'BOARDING', 'DEPARTED', 'IN_FLIGHT', 'LANDED', 'CANCELLED') DEFAULT 'SCHEDULED',
    FOREIGN KEY (aircraft_id) REFERENCES aircraft(id),
    CHECK (arrival_time > departure_time),
    CHECK (departure_code != arrival_code)
);

-- Passenger table
CREATE TABLE passenger (
    id INT AUTO_INCREMENT PRIMARY KEY,
    passport_number VARCHAR(20) UNIQUE,
    name VARCHAR(100) NOT NULL,
    email VARCHAR(254) NOT NULL,
    phone VARCHAR(15) NOT NULL,
    address VARCHAR(100)
);

-- Ticket table
CREATE TABLE ticket (
    id INT AUTO_INCREMENT PRIMARY KEY,
    ticket_number VARCHAR(20) NOT NULL UNIQUE,
    flight_id INT NOT NULL,
    passenger_id INT NOT NULL,
    seat_number VARCHAR(4) NOT NULL,
    price DECIMAL(10,2) NOT NULL CHECK (price >= 0),
    currency VARCHAR(3) NOT NULL,
    status ENUM('PENDING', 'CONFIRMED', 'CHECKED_IN', 'BOARDED', 'COMPLETED', 'CANCELLED', 'REFUNDED') DEFAULT 'PENDING',
    FOREIGN KEY (flight_id) REFERENCES flight(id),
    FOREIGN KEY (passenger_id) REFERENCES passenger(id),
    UNIQUE KEY unique_flight_seat (flight_id, seat_number)
);

-- Insert default seat classes
INSERT INTO seat_class (code, name) VALUES
('E', 'ECONOMY'),
('B', 'BUSINESS'),
('F', 'FIRST');

-- Create indexes for better performance
CREATE INDEX idx_flight_dates ON flight(departure_time, arrival_time);
CREATE INDEX idx_ticket_flight ON ticket(flight_id);
CREATE INDEX idx_ticket_passenger ON ticket(passenger_id);
CREATE INDEX idx_aircraft_seat_layout ON aircraft_seat_layout(aircraft_id);

-- Đăng nhập với tài khoản admin/root

-- Sau khi đăng nhập, cấp quyền SUPER cho tài khoản của bạn
FLUSH PRIVILEGES;
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
END //
DELIMITER ;

-- Insert data into aircraft table
INSERT INTO aircraft (serial_number, model, economy_seats, business_seats, first_seats) VALUES
('VN123', 'Boeing 787-9', 214, 28, 8),
('VN124', 'Airbus A350-900', 226, 32, 0),
('VN125', 'Airbus A321neo', 162, 16, 0),
('VN126', 'Boeing 777-300ER', 268, 48, 4),
('VN127', 'Airbus A330-300', 256, 24, 0);

-- Insert data into flight table (merged with route)
INSERT INTO flight (flight_number, departure_code, departure_name, arrival_code, arrival_name, aircraft_id, departure_time, arrival_time, status) VALUES
('VN100', 'SGN', 'Tan Son Nhat', 'HAN', 'Noi Bai', 1, '2025-05-22 07:30:00', '2025-05-22 09:30:00', 'SCHEDULED'),
('VN101', 'HAN', 'Noi Bai', 'SGN', 'Tan Son Nhat', 1, '2025-05-22 19:00:00', '2025-05-22 21:00:00', 'SCHEDULED'),
('VN200', 'SGN', 'Tan Son Nhat', 'DAD', 'Da Nang', 3, '2025-05-22 08:45:00', '2025-05-22 10:00:00', 'SCHEDULED'),
('VN201', 'HAN', 'Noi Bai', 'DAD', 'Da Nang', 3, '2025-05-22 14:30:00', '2025-05-22 15:45:00', 'SCHEDULED'),
('VN300', 'SGN', 'Tan Son Nhat', 'PQC', 'Phu Quoc', 3, '2025-05-23 10:15:00', '2025-05-23 11:30:00', 'SCHEDULED'),
('VN301', 'HAN', 'Noi Bai', 'PQC', 'Phu Quoc', 2, '2025-05-23 12:00:00', '2025-05-23 14:00:00', 'SCHEDULED'),
('VN400', 'SGN', 'Tan Son Nhat', 'CXR', 'Cam Ranh', 3, '2025-05-24 06:30:00', '2025-05-24 07:45:00', 'SCHEDULED'),
('VN500', 'SGN', 'Tan Son Nhat', 'ICN', 'Incheon', 2, '2025-05-25 00:30:00', '2025-05-25 07:30:00', 'SCHEDULED'),
('VN600', 'HAN', 'Noi Bai', 'NRT', 'Narita', 4, '2025-05-25 08:00:00', '2025-05-25 15:00:00', 'SCHEDULED'),
('VN102', 'SGN', 'Tan Son Nhat', 'HAN', 'Noi Bai', 1, '2025-05-23 07:30:00', '2025-05-23 09:30:00', 'SCHEDULED'),
('VN103', 'HAN', 'Noi Bai', 'SGN', 'Tan Son Nhat', 1, '2025-05-23 19:00:00', '2025-05-23 21:00:00', 'SCHEDULED'),
('VN104', 'SGN', 'Tan Son Nhat', 'HAN', 'Noi Bai', 1, '2025-05-24 07:30:00', '2025-05-24 09:30:00', 'SCHEDULED');

-- Insert data into passenger table
INSERT INTO passenger (passport_number, name, email, phone, address) VALUES
('VN:1123456', 'Nguyen Van A', 'nguyenvana@email.com', '+84901234567', '123 Nguyen Hue, District 1, Ho Chi Minh City'),
('VN:1234567', 'Tran Thi B', 'tranthib@email.com', '+84912345678', '456 Le Loi, District 1, Ho Chi Minh City'),
('VN:1345678', 'Le Van C', 'levanc@email.com', '+84923456789', '789 Dong Khoi, District 1, Ho Chi Minh City'),
('VN:1456789', 'Pham Thi D', 'phamthid@email.com', '+84934567890', '101 Tran Hung Dao, District 1, Ho Chi Minh City'),
('VN:1567890', 'Hoang Van E', 'hoangvane@email.com', '+84945678901', '202 Hai Ba Trung, District 1, Ho Chi Minh City'),
('VN:1678901', 'Vu Thi F', 'vuthif@email.com', '+84956789012', '15 Nguyen Trai, District 5, Ho Chi Minh City'),
('VN:1789012', 'Dang Van G', 'dangvang@email.com', '+84967890123', '25 Ly Tu Trong, District 1, Ho Chi Minh City'),
('VN:1890123', 'Lam Thi H', 'lamthih@email.com', '+84978901234', '35 Cong Quynh, District 1, Ho Chi Minh City'),
('VN:1901234', 'Trinh Van I', 'trinhvani@email.com', '+84989012345', '45 Vo Van Tan, District 3, Ho Chi Minh City'),
('VN:1012345', 'Mai Thi K', 'maithik@email.com', '+84990123456', '55 Dien Bien Phu, District 3, Ho Chi Minh City');

-- Insert data into ticket table
INSERT INTO ticket (ticket_number, flight_id, passenger_id, seat_number, price, currency, status) VALUES
-- VN100 (SGN-HAN) on VN123
('VN100-20250522-0001', 1, 1, 'F01', 5000000, 'VND', 'CONFIRMED'),
('VN100-20250522-0002', 1, 2, 'B05', 3500000, 'VND', 'CONFIRMED'),
('VN100-20250522-0003', 1, 3, 'E120', 1800000, 'VND', 'CONFIRMED'),
('VN100-20250522-0004', 1, 4, 'E121', 1800000, 'VND', 'CONFIRMED'),

-- VN101 (HAN-SGN) on VN123
('VN101-20250522-0001', 2, 5, 'B12', 3600000, 'VND', 'CONFIRMED'),
('VN101-20250522-0002', 2, 6, 'E045', 1900000, 'VND', 'CONFIRMED'),

-- VN200 (SGN-DAD) on VN125
('VN200-20250522-0001', 3, 7, 'B08', 2800000, 'VND', 'CHECKED_IN'),
('VN200-20250522-0002', 3, 8, 'E078', 1500000, 'VND', 'CHECKED_IN'),

-- VN500 (SGN-ICN) on VN124
('VN500-20250525-0001', 8, 9, 'B15', 8500000, 'VND', 'CONFIRMED'),
('VN500-20250525-0002', 8, 10, 'E156', 5200000, 'VND', 'CONFIRMED'),

-- VN600 (HAN-NRT) on VN126
('VN600-20250525-0001', 9, 1, 'F02', 12000000, 'VND', 'PENDING'),
('VN600-20250525-0002', 9, 2, 'B22', 8500000, 'VND', 'PENDING'),

-- Add tickets for future flights
-- VN102 (SGN-HAN) tomorrow on VN123
('VN102-20250523-0001', 10, 3, 'B02', 3500000, 'VND', 'CONFIRMED'),
('VN102-20250523-0002', 10, 4, 'E025', 1800000, 'VND', 'CONFIRMED'),

-- VN103 (HAN-SGN) tomorrow on VN123
('VN103-20250523-0001', 11, 5, 'B08', 3600000, 'VND', 'CONFIRMED'),
('VN103-20250523-0002', 11, 6, 'E110', 1900000, 'VND', 'CONFIRMED'),

-- VN104 (SGN-HAN) day after tomorrow on VN123
('VN104-20250524-0001', 12, 7, 'F03', 5000000, 'VND', 'PENDING');

-- Add some statistics data
-- Insert additional passengers (bulk data)
INSERT INTO passenger (passport_number, name, email, phone, address)
WITH RECURSIVE numbers AS (
    SELECT 1 AS n
    UNION ALL
    SELECT n + 1 FROM numbers WHERE n < 100
)
SELECT
    CONCAT('BLK', LPAD(n, 6, '0')),
    CONCAT('Test Passenger ', n),
    CONCAT('test', n, '@example.com'),
    CONCAT('+8498', LPAD(n, 7, '0')),
    CONCAT(n, ' Test Street, Test City')
FROM numbers;