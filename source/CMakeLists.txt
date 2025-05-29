cmake_minimum_required(VERSION 3.10)
project(AirlinesManagement)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Qt paths for macOS
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@6/6.6.1")
if(NOT EXISTS ${CMAKE_PREFIX_PATH})
    set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.10")
endif()

# Find Qt packages
find_package(Qt6 COMPONENTS Core Widgets REQUIRED)
if (NOT Qt6_FOUND)
    find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
endif()

# Set Qt MOC and RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find source files
file(GLOB_RECURSE CORE_SOURCES ${CMAKE_SOURCE_DIR}/src/core/*.cpp)
file(GLOB_RECURSE DATABASE_SOURCES ${CMAKE_SOURCE_DIR}/src/database/*.cpp)
file(GLOB_RECURSE REPOSITORY_SOURCES ${CMAKE_SOURCE_DIR}/src/repositories/*.cpp)
file(GLOB_RECURSE SERVICES_SOURCES ${CMAKE_SOURCE_DIR}/src/services/*.cpp)
file(GLOB_RECURSE UTILS_SOURCES ${CMAKE_SOURCE_DIR}/src/utils/*.cpp)
file(GLOB_RECURSE UI_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/ui/*.cpp 
    ${CMAKE_SOURCE_DIR}/src/ui/*.h
    ${CMAKE_SOURCE_DIR}/src/ui/*.ui
)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Find MySQL Connector/C++
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
    message(FATAL_ERROR "MySQL Connector/C++ library not found. Please install with: brew install mysql-connector-c++")
endif()

if(NOT MYSQLCPPCONN_INCLUDE_DIR)
    message(FATAL_ERROR "MySQL Connector/C++ include directory not found. Please install with: brew install mysql-connector-c++")
endif()

include_directories(${MYSQLCPPCONN_INCLUDE_DIR})

# Create libraries
add_library(utils_lib STATIC ${UTILS_SOURCES})
add_library(database_lib STATIC ${DATABASE_SOURCES})
add_library(core_lib STATIC ${CORE_SOURCES})
add_library(repository_lib STATIC ${REPOSITORY_SOURCES})
add_library(services_lib STATIC ${SERVICES_SOURCES})

# Link libraries
target_link_libraries(database_lib
    ${MYSQLCPPCONN_LIBRARY}
    utils_lib
)

target_link_libraries(core_lib
    utils_lib
)

target_link_libraries(repository_lib
    core_lib
    database_lib
    utils_lib
)

target_link_libraries(services_lib
    repository_lib
    core_lib
    database_lib
    utils_lib
)

# Create UI executable
add_executable(${PROJECT_NAME}_ui
    src/main.cpp
    ${UI_SOURCES}
)

target_link_libraries(${PROJECT_NAME}_ui
    Qt::Core
    Qt::Widgets
    services_lib
    repository_lib
    core_lib
    database_lib
    utils_lib
    ${MYSQLCPPCONN_LIBRARY}
) 