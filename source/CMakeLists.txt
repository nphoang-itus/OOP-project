cmake_minimum_required(VERSION 3.10)
project(AirlinesManagement)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tìm tất cả các file nguồn
file(GLOB_RECURSE CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
file(GLOB_RECURSE DATABASE_SOURCES ${CMAKE_SOURCE_DIR}/src/database/*.cpp)
file(GLOB_RECURSE REPOSITORY_SOURCES ${CMAKE_SOURCE_DIR}/src/repositories/*.cpp)
file(GLOB_RECURSE SERVICES_SOURCES ${CMAKE_SOURCE_DIR}/src/services/*.cpp)
file(GLOB_RECURSE UTILS_SOURCES ${CMAKE_SOURCE_DIR}/src/utils/*.cpp)
file(GLOB_RECURSE UI_SOURCES ${CMAKE_SOURCE_DIR}/src/ui/*.cpp)

# Thêm thư mục chứa file header
include_directories(${CMAKE_SOURCE_DIR}/src)

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

# Cấu hình wxWidgets cho UI
find_package(wxWidgets REQUIRED COMPONENTS core base xrc html qa)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    message(STATUS "wxWidgets found: ${wxWidgets_LIBRARIES}")
else()
    message(FATAL_ERROR "wxWidgets not found")
endif()

# Thêm các đường dẫn include cho wxWidgets (nếu cần)
include_directories(
    ${CMAKE_SOURCE_DIR}/src/ui
    /opt/homebrew/include/wx-3.2
    /opt/homebrew/lib/wx/include/osx_cocoa-unicode-3.2
)

# Thêm đường dẫn thư viện wxWidgets (nếu cần)
link_directories(/opt/homebrew/lib)

# Tạo executable cho ứng dụng
add_executable(airlines_app
    src/main.cpp
    ${CORE_SOURCES}
    ${DATABASE_SOURCES}
    ${REPOSITORY_SOURCES}
    ${SERVICES_SOURCES}
    ${UTILS_SOURCES}
    ${UI_SOURCES}
)

target_link_libraries(airlines_app 
    ${wxWidgets_LIBRARIES}
    ${MYSQLCPPCONN_LIBRARY}
)
