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

-- Flight seat availability table
CREATE TABLE flight_seat_availability (
    id INT AUTO_INCREMENT PRIMARY KEY,
    flight_id INT NOT NULL,
    seat_number VARCHAR(4) NOT NULL,
    is_available BOOLEAN NOT NULL DEFAULT TRUE,
    FOREIGN KEY (flight_id) REFERENCES flight(id),
    UNIQUE KEY unique_flight_seat (flight_id, seat_number)
);

-- Create index for better performance
CREATE INDEX idx_flight_seat_availability ON flight_seat_availability(flight_id);

-- Insert data into flight_seat_availability table
INSERT INTO flight_seat_availability (flight_id, seat_number, is_available)
SELECT f.id, 
       CONCAT(c.code, LPAD(n.n, 
           CASE 
               WHEN c.code = 'E' AND a.economy_seats > 99 THEN 3  -- E001-E999
               WHEN c.code = 'B' AND a.business_seats > 99 THEN 3  -- B001-B999
               WHEN c.code = 'F' AND a.first_seats > 99 THEN 3    -- F001-F999
               WHEN c.code = 'E' AND a.economy_seats > 9 THEN 2   -- E01-E99
               WHEN c.code = 'B' AND a.business_seats > 9 THEN 2  -- B01-B99
               WHEN c.code = 'F' AND a.first_seats > 9 THEN 2     -- F01-F99
               ELSE 1  -- Single digit padding for small numbers
           END, '0')) as seat_number,
       TRUE as is_available
FROM flight f
CROSS JOIN seat_class c
CROSS JOIN (
    SELECT 1 as n UNION SELECT 2 UNION SELECT 3 UNION SELECT 4 UNION SELECT 5
    UNION SELECT 6 UNION SELECT 7 UNION SELECT 8 UNION SELECT 9 UNION SELECT 10
    UNION SELECT 11 UNION SELECT 12 UNION SELECT 13 UNION SELECT 14 UNION SELECT 15
    UNION SELECT 16 UNION SELECT 17 UNION SELECT 18 UNION SELECT 19 UNION SELECT 20
    UNION SELECT 21 UNION SELECT 22 UNION SELECT 23 UNION SELECT 24 UNION SELECT 25
    UNION SELECT 26 UNION SELECT 27 UNION SELECT 28 UNION SELECT 29 UNION SELECT 30
    UNION SELECT 31 UNION SELECT 32 UNION SELECT 33 UNION SELECT 34 UNION SELECT 35
    UNION SELECT 36 UNION SELECT 37 UNION SELECT 38 UNION SELECT 39 UNION SELECT 40
    UNION SELECT 41 UNION SELECT 42 UNION SELECT 43 UNION SELECT 44 UNION SELECT 45
    UNION SELECT 46 UNION SELECT 47 UNION SELECT 48 UNION SELECT 49 UNION SELECT 50
    UNION SELECT 51 UNION SELECT 52 UNION SELECT 53 UNION SELECT 54 UNION SELECT 55
    UNION SELECT 56 UNION SELECT 57 UNION SELECT 58 UNION SELECT 59 UNION SELECT 60
    UNION SELECT 61 UNION SELECT 62 UNION SELECT 63 UNION SELECT 64 UNION SELECT 65
    UNION SELECT 66 UNION SELECT 67 UNION SELECT 68 UNION SELECT 69 UNION SELECT 70
    UNION SELECT 71 UNION SELECT 72 UNION SELECT 73 UNION SELECT 74 UNION SELECT 75
    UNION SELECT 76 UNION SELECT 77 UNION SELECT 78 UNION SELECT 79 UNION SELECT 80
    UNION SELECT 81 UNION SELECT 82 UNION SELECT 83 UNION SELECT 84 UNION SELECT 85
    UNION SELECT 86 UNION SELECT 87 UNION SELECT 88 UNION SELECT 89 UNION SELECT 90
    UNION SELECT 91 UNION SELECT 92 UNION SELECT 93 UNION SELECT 94 UNION SELECT 95
    UNION SELECT 96 UNION SELECT 97 UNION SELECT 98 UNION SELECT 99 UNION SELECT 100
    UNION SELECT 101 UNION SELECT 102 UNION SELECT 103 UNION SELECT 104 UNION SELECT 105
    UNION SELECT 106 UNION SELECT 107 UNION SELECT 108 UNION SELECT 109 UNION SELECT 110
    UNION SELECT 111 UNION SELECT 112 UNION SELECT 113 UNION SELECT 114 UNION SELECT 115
    UNION SELECT 116 UNION SELECT 117 UNION SELECT 118 UNION SELECT 119 UNION SELECT 120
    UNION SELECT 121 UNION SELECT 122 UNION SELECT 123 UNION SELECT 124 UNION SELECT 125
    UNION SELECT 126 UNION SELECT 127 UNION SELECT 128 UNION SELECT 129 UNION SELECT 130
    UNION SELECT 131 UNION SELECT 132 UNION SELECT 133 UNION SELECT 134 UNION SELECT 135
    UNION SELECT 136 UNION SELECT 137 UNION SELECT 138 UNION SELECT 139 UNION SELECT 140
    UNION SELECT 141 UNION SELECT 142 UNION SELECT 143 UNION SELECT 144 UNION SELECT 145
    UNION SELECT 146 UNION SELECT 147 UNION SELECT 148 UNION SELECT 149 UNION SELECT 150
    UNION SELECT 151 UNION SELECT 152 UNION SELECT 153 UNION SELECT 154 UNION SELECT 155
    UNION SELECT 156 UNION SELECT 157 UNION SELECT 158 UNION SELECT 159 UNION SELECT 160
    UNION SELECT 161 UNION SELECT 162 UNION SELECT 163 UNION SELECT 164 UNION SELECT 165
    UNION SELECT 166 UNION SELECT 167 UNION SELECT 168 UNION SELECT 169 UNION SELECT 170
    UNION SELECT 171 UNION SELECT 172 UNION SELECT 173 UNION SELECT 174 UNION SELECT 175
    UNION SELECT 176 UNION SELECT 177 UNION SELECT 178 UNION SELECT 179 UNION SELECT 180
    UNION SELECT 181 UNION SELECT 182 UNION SELECT 183 UNION SELECT 184 UNION SELECT 185
    UNION SELECT 186 UNION SELECT 187 UNION SELECT 188 UNION SELECT 189 UNION SELECT 190
    UNION SELECT 191 UNION SELECT 192 UNION SELECT 193 UNION SELECT 194 UNION SELECT 195
    UNION SELECT 196 UNION SELECT 197 UNION SELECT 198 UNION SELECT 199 UNION SELECT 200
    UNION SELECT 201 UNION SELECT 202 UNION SELECT 203 UNION SELECT 204 UNION SELECT 205
    UNION SELECT 206 UNION SELECT 207 UNION SELECT 208 UNION SELECT 209 UNION SELECT 210
    UNION SELECT 211 UNION SELECT 212 UNION SELECT 213 UNION SELECT 214 UNION SELECT 215
    UNION SELECT 216 UNION SELECT 217 UNION SELECT 218 UNION SELECT 219 UNION SELECT 220
    UNION SELECT 221 UNION SELECT 222 UNION SELECT 223 UNION SELECT 224 UNION SELECT 225
    UNION SELECT 226 UNION SELECT 227 UNION SELECT 228 UNION SELECT 229 UNION SELECT 230
    UNION SELECT 231 UNION SELECT 232 UNION SELECT 233 UNION SELECT 234 UNION SELECT 235
    UNION SELECT 236 UNION SELECT 237 UNION SELECT 238 UNION SELECT 239 UNION SELECT 240
    UNION SELECT 241 UNION SELECT 242 UNION SELECT 243 UNION SELECT 244 UNION SELECT 245
    UNION SELECT 246 UNION SELECT 247 UNION SELECT 248 UNION SELECT 249 UNION SELECT 250
) n
JOIN aircraft a ON f.aircraft_id = a.id
JOIN aircraft_seat_layout asl ON a.id = asl.aircraft_id AND c.code = asl.seat_class_code
WHERE n.n <= CASE 
    WHEN c.code = 'E' THEN a.economy_seats
    WHEN c.code = 'B' THEN a.business_seats
    WHEN c.code = 'F' THEN a.first_seats
END;

-- Update existing tickets to mark their seats as unavailable
UPDATE flight_seat_availability fsa
JOIN ticket t ON fsa.flight_id = t.flight_id AND fsa.seat_number = t.seat_number
SET fsa.is_available = FALSE;

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
('VN123', 'Boeing 787-9', 200, 50, 20),
('VN124', 'Airbus A350-900', 200, 50, 20),
('VN125', 'Airbus A321neo', 150, 30, 10),
('VN126', 'Boeing 777-300ER', 250, 60, 30),
('VN127', 'Airbus A330-300', 180, 40, 15);

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
('VN100-20250522-0002', 1, 2, 'B01', 3500000, 'VND', 'CONFIRMED'),
('VN100-20250522-0003', 1, 3, 'E001', 1800000, 'VND', 'CONFIRMED'),
('VN100-20250522-0004', 1, 4, 'E002', 1800000, 'VND', 'CONFIRMED'),

-- VN101 (HAN-SGN) on VN123
('VN101-20250522-0001', 2, 5, 'B02', 3600000, 'VND', 'CONFIRMED'),
('VN101-20250522-0002', 2, 6, 'E003', 1900000, 'VND', 'CONFIRMED'),

-- VN200 (SGN-DAD) on VN125
('VN200-20250522-0001', 3, 7, 'B03', 2800000, 'VND', 'CHECKED_IN'),
('VN200-20250522-0002', 3, 8, 'E004', 1500000, 'VND', 'CHECKED_IN'),

-- VN500 (SGN-ICN) on VN124
('VN500-20250525-0001', 8, 9, 'B04', 8500000, 'VND', 'CONFIRMED'),
('VN500-20250525-0002', 8, 10, 'E005', 5200000, 'VND', 'CONFIRMED'),

-- VN600 (HAN-NRT) on VN126
('VN600-20250525-0001', 9, 1, 'F02', 12000000, 'VND', 'PENDING'),
('VN600-20250525-0002', 9, 2, 'B05', 8500000, 'VND', 'PENDING'),

-- Add tickets for future flights
-- VN102 (SGN-HAN) tomorrow on VN123
('VN102-20250523-0001', 10, 3, 'B06', 3500000, 'VND', 'CONFIRMED'),
('VN102-20250523-0002', 10, 4, 'E006', 1800000, 'VND', 'CONFIRMED'),

-- VN103 (HAN-SGN) tomorrow on VN123
('VN103-20250523-0001', 11, 5, 'B07', 3600000, 'VND', 'CONFIRMED'),
('VN103-20250523-0002', 11, 6, 'E007', 1900000, 'VND', 'CONFIRMED'),

-- VN104 (SGN-HAN) day after tomorrow on VN123
('VN104-20250524-0001', 12, 7, 'F03', 5000000, 'VND', 'PENDING');