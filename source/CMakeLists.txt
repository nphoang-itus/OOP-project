cmake_minimum_required(VERSION 3.10)
project(airlines_app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
file(GLOB_RECURSE DATABASE_SOURCES ${CMAKE_SOURCE_DIR}/src/database/*.cpp)
file(GLOB_RECURSE UTIL_SOURCES ${CMAKE_SOURCE_DIR}/src/utils/*.cpp)

# Tìm MySQL Connector/C++
find_library(MYSQLCPPCONN_LIBRARY
    NAMES mysqlcppconn
    HINTS /usr/local/Cellar/mysql-connector-cpp/*/lib
          /usr/local/mysql-connector-c++-8.0.33/lib64
          /usr/lib
          /usr/local/lib
)

if (NOT MYSQLCPPCONN_LIBRARY)
    message(FATAL_ERROR "Could not find MYSQLCPPCONN_LIBRARY.")
endif()

find_path(MYSQLCPPCONN_INCLUDE_DIR
    NAMES mysql_driver.h
    HINTS /usr/local/Cellar/mysql-connector-cpp/*/include
          /usr/local/mysql-connector-c++-8.0.33/include/jdbc
          /usr/include/mysql-cppconn
          /usr/local/include
)

if (NOT MYSQLCPPCONN_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find MYSQLCPPCONN_INCLUDE_DIR.")
endif()

include_directories(${MYSQLCPPCONN_INCLUDE_DIR})

# Thêm các file nguồn
add_executable(main
    src/main.cpp
    ${CORE_SOURCES}
    ${DATABASE_SOURCES}
    ${UTIL_SOURCES}
)

# Link thư viện
target_link_libraries(main PRIVATE ${MYSQLCPPCONN_LIBRARY})