DELETE FROM aircraft WHERE serial_number = 'VN123'
select * from aircraft

SELECT a.id, a.serial_number, a.model, COALESCE(GROUP_CONCAT(CONCAT(sc.code, ':', asl.seat_count) ORDER BY sc.code), '') as seat_layout FROM aircraft a LEFT JOIN aircraft_seat_layout asl ON a.id = asl.aircraft_id LEFT JOIN seat_class sc ON asl.seat_class_code = sc.code WHERE a.id = '1' GROUP BY a.id, a.serial_number, a.model

UPDATE aircraft SET serial_number = 'VN333', model = 'Boeing 737', economy_seats = '100', business_seats = '20', first_seats = '10' WHERE id = '999999'

INSERT INTO passenger (passport_number, name, email, phone, address) VALUES ('VN:12345678', 'John Doe', 'john.doe@example.com', '1234567890', '123 Main St')