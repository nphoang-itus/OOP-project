cmake_minimum_required(VERSION 3.10)
project(AirlinesManagement)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Thêm thư mục chứa file header
include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_SOURCE_DIR}/src/database)
include_directories(${CMAKE_SOURCE_DIR}/src/repositories)
include_directories(${CMAKE_SOURCE_DIR}/src/utils)

# Tìm thư viện MySQL Connector/C++
find_library(MYSQLCPPCONN_LIBRARY NAMES mysqlcppconn libmysqlcppconn
    HINTS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
)
find_path(MYSQLCPPCONN_INCLUDE_DIR NAMES cppconn/driver.h
    HINTS /usr/include /usr/local/include
)

if(NOT MYSQLCPPCONN_LIBRARY OR NOT MYSQLCPPCONN_INCLUDE_DIR)
    message(FATAL_ERROR "MySQL Connector/C++ not found. Please install libmysqlcppconn-dev.")
endif()

include_directories(${MYSQLCPPCONN_INCLUDE_DIR})

# Tạo executable
add_executable(main
    src/main.cpp
    src/core/Flight.cpp
    src/core/Passenger.cpp
    src/core/Reservation.cpp
    src/database/MySQLConnection.cpp
    src/repositories/FlightRepository.cpp
)

# Liên kết thư viện MySQL Connector/C++
target_link_libraries(main ${MYSQLCPPCONN_LIBRARY})