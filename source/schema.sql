-- Create database if it doesn't exist
CREATE DATABASE IF NOT EXISTS airlines_db;
USE airlines_db;

-- Flight details table
CREATE TABLE IF NOT EXISTS flight_tb (
    f_id INT AUTO_INCREMENT PRIMARY KEY,
    f_no VARCHAR(10) NOT NULL,
    f_name VARCHAR(50) NOT NULL,
    f_from VARCHAR(50) NOT NULL,
    f_destination VARCHAR(50) NOT NULL,
    f_departureTime DATETIME NOT NULL,
    f_leaveTime DATETIME,
    f_arrivalTime DATETIME,
    f_amount DOUBLE NOT NULL,
    f_availability CHAR(1) NOT NULL DEFAULT 'A',
    CHECK (f_availability IN ('A', 'N')),
    UNIQUE (f_no)
);

-- Passenger table
CREATE TABLE IF NOT EXISTS passenger_tb (
    p_id INT AUTO_INCREMENT PRIMARY KEY,
    p_name VARCHAR(100) NOT NULL,
    p_phone VARCHAR(20) NOT NULL,
    p_passport VARCHAR(20) NOT NULL,
    p_address VARCHAR(200),
    UNIQUE (p_passport)
);

-- Reservation table
CREATE TABLE IF NOT EXISTS reservation_tb (
    r_id INT AUTO_INCREMENT PRIMARY KEY,
    r_ticketNo VARCHAR(20) NOT NULL,
    r_flightNo VARCHAR(10) NOT NULL,
    r_passportPassenger VARCHAR(20) NOT NULL,
    UNIQUE (r_ticketNo),
    FOREIGN KEY (r_flightNo) REFERENCES flight_tb(f_no),
    FOREIGN KEY (r_passportPassenger) REFERENCES passenger_tb(p_passport)
);

-- Insert sample data
INSERT INTO flight_tb (f_no, f_name, f_from, f_destination, f_departureTime, f_leaveTime, f_arrivalTime, f_amount, f_availability)
VALUES 
('FL007', 'Jetstar', 'Da Nang', 'Phu Quoc', '2025-05-02 09:11:12', '2025-05-02 08:30:00', '2025-05-02 11:00:00', 150.00, 'A'),
('FL008', 'Bamboo Airways', 'Hue', 'Nha Trang', '2024-05-03 14:00:00', '2025-05-03 13:30:00', '2025-05-03 16:00:00', 120.00, 'A'),
('FL009', 'Vietnam Airlines', 'Ho Chi Minh City', 'Hanoi', '2026-05-04 06:00:00', '2025-05-04 05:30:00', '2025-05-04 08:30:00', 200.00, 'A'),
('FL010', 'Jetstar', 'Can Tho', 'Da Nang', '2028-05-05 10:00:00', '2025-05-05 09:30:00', '2025-05-05 12:00:00', 180.00, 'N'),
('FL011', 'Bamboo Airways', 'Phu Quoc', 'Ho Chi Minh City', '2012-05-06 15:00:00', '2025-05-06 14:30:00', '2025-05-06 16:30:00', 130.00, 'A'),
('FL012', 'Vietnam Airlines', 'Hanoi', 'Da Nang', '2024-05-07 07:00:00', '2025-05-07 06:30:00', '2025-05-07 09:00:00', 170.00, 'A'),
('FL013', 'Jetstar', 'Nha Trang', 'Hue', '2025-05-08 12:00:00', '2025-05-08 11:30:00', '2025-05-08 14:00:00', 140.00, 'N'),
('FL014', 'Bamboo Airways', 'Ho Chi Minh City', 'Da Nang', '2025-05-09 18:00:00', '2025-05-09 17:30:00', '2025-05-09 20:00:00', 160.00, 'A'),
('FL015', 'Vietnam Airlines', 'Hanoi', 'Phu Quoc', '2025-05-10 05:00:00', '2025-05-10 04:30:00', '2025-05-10 08:00:00', 250.00, 'A'),
('FL016', 'Jetstar', 'Da Nang', 'Ho Chi Minh City', '2025-05-11 20:00:00', '2025-05-11 19:30:00', '2025-05-11 22:00:00', 190.00, 'A');