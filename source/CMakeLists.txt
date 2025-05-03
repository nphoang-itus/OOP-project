cmake_minimum_required(VERSION 3.10)
project(AirlinesManagement)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tự động tìm tất cả các file header và source
file(GLOB_RECURSE CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
file(GLOB_RECURSE DATABASE_SOURCES ${CMAKE_SOURCE_DIR}/src/database/*.cpp)
file(GLOB_RECURSE REPOSITORY_SOURCES ${CMAKE_SOURCE_DIR}/src/repositories/*.cpp)
file(GLOB_RECURSE UTILS_SOURCES ${CMAKE_SOURCE_DIR}/src/utils/*.cpp)

# Thêm thư mục chứa file header
include_directories(${CMAKE_SOURCE_DIR})

# Tìm thư viện MySQL Connector/C++
find_library(MYSQLCPPCONN_LIBRARY 
    NAMES mysqlcppconnx libmysqlcppconnx
    HINTS 
        /usr/local/Cellar/mysql-connector-c++/9.3.0/lib
        /usr/local/lib 
        /usr/local/opt/mysql-connector-c++/lib
        /opt/homebrew/lib
)

find_path(MYSQLCPPCONN_INCLUDE_DIR 
    NAMES mysqlx/xdevapi.h
    HINTS
        /usr/local/Cellar/mysql-connector-c++/9.3.0/include
        /usr/local/include 
        /usr/local/opt/mysql-connector-c++/include
        /opt/homebrew/include
)

if(NOT MYSQLCPPCONN_LIBRARY)
    message(FATAL_ERROR "Không tìm thấy thư viện MySQL Connector/C++. Vui lòng cài đặt với lệnh: brew install mysql-connector-c++")
endif()

if(NOT MYSQLCPPCONN_INCLUDE_DIR)
    message(FATAL_ERROR "Không tìm thấy thư mục include MySQL Connector/C++. Vui lòng cài đặt với lệnh: brew install mysql-connector-c++")
endif()

include_directories(${MYSQLCPPCONN_INCLUDE_DIR})

# Tạo executable
add_executable(main
    src/main.cpp
    ${CORE_SOURCES}
    ${DATABASE_SOURCES}
    ${REPOSITORY_SOURCES}
    ${UTILS_SOURCES}
)

# Liên kết thư viện MySQL Connector/C++
target_link_libraries(main ${MYSQLCPPCONN_LIBRARY})