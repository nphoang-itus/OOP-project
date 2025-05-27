cmake_minimum_required(VERSION 3.10)
project(AirlinesManagement)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable testing
enable_testing()

# Find all source files
file(GLOB_RECURSE CORE_SOURCES 
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Logger.cpp
)
file(GLOB_RECURSE REPOSITORY_SOURCES ${CMAKE_SOURCE_DIR}/src/repositories/MockRepository/*.cpp)
file(GLOB_RECURSE SERVICE_SOURCES ${CMAKE_SOURCE_DIR}/src/services/*.cpp)
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/src/tests/mock/*.cpp)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/src)

# Create core library
add_library(core_lib STATIC
    ${CORE_SOURCES}
)

# Create repository library
add_library(repository_lib STATIC
    ${REPOSITORY_SOURCES}
)

# Create service library
add_library(service_lib STATIC
    ${SERVICE_SOURCES}
)

# Configure GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Create test executable for mock repository tests
add_executable(${PROJECT_NAME}_mock_test
    ${TEST_SOURCES}
)

# Create test executable for service tests
add_executable(${PROJECT_NAME}_service_test
    # src/tests/services/AircraftServiceTest.cpp
    # src/tests/services/FlightServiceTest.cpp
    # src/tests/services/PassengerServiceTest.cpp
    src/tests/services/TicketServiceTest.cpp
)

# Link libraries for mock repository tests
target_link_libraries(${PROJECT_NAME}_mock_test
    GTest::gtest_main
    pthread
    core_lib
    repository_lib
)

# Link libraries for service tests
target_link_libraries(${PROJECT_NAME}_service_test
    GTest::gtest_main
    pthread
    core_lib
    repository_lib
    service_lib
)

# Add tests to CTest
add_test(NAME MockRepositoryTest COMMAND ${PROJECT_NAME}_mock_test)
add_test(NAME AircraftServiceTest COMMAND ${PROJECT_NAME}_service_test)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra") 