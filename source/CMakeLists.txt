cmake_minimum_required(VERSION 3.14)
project(OOP_Project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find GTest package
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add test executable
add_executable(${PROJECT_NAME}_test
    src/tests/core/value_objects/BaseValueObjectTest.h
    src/tests/core/value_objects/AircraftSerialTest.cpp
    src/tests/core/value_objects/SeatClassMapTest.cpp
    src/tests/core/value_objects/PassportNumberTest.cpp
)

# Link test executable with GTest
target_link_libraries(${PROJECT_NAME}_test
    GTest::gtest_main
    pthread
)

# Include directories for tests
target_include_directories(${PROJECT_NAME}_test PRIVATE
    ${CMAKE_SOURCE_DIR}
)

# Enable testing
enable_testing()
add_test(NAME ${PROJECT_NAME}_test COMMAND ${PROJECT_NAME}_test) 