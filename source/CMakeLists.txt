cmake_minimum_required(VERSION 3.10)
project(AirlinesManagement VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Platform-specific settings
if(APPLE)
    set(MYSQLCPPCONN_PATHS
        /usr/local/Cellar/mysql-connector-c++/9.3.0
        /usr/local/opt/mysql-connector-c++
        /opt/homebrew/opt/mysql-connector-c++
        # Additional paths
        /opt/homebrew/Cellar/mysql-connector-c++
        $ENV{HOME}/homebrew/opt/mysql-connector-c++
        $ENV{HOME}/.homebrew/opt/mysql-connector-c++
    )
    # Add Homebrew prefix if available
    execute_process(
        COMMAND brew --prefix
        OUTPUT_VARIABLE HOMEBREW_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(HOMEBREW_PREFIX)
        list(APPEND MYSQLCPPCONN_PATHS "${HOMEBREW_PREFIX}/opt/mysql-connector-c++")
    endif()
elseif(WIN32)
    set(MYSQLCPPCONN_PATHS
        "C:/Program Files/MySQL/MySQL Connector C++"
        "C:/Program Files (x86)/MySQL/MySQL Connector C++"
        # Additional paths
        "$ENV{ProgramFiles}/MySQL/MySQL Connector C++"
        "$ENV{ProgramFiles(x86)}/MySQL/MySQL Connector C++"
        "$ENV{LOCALAPPDATA}/MySQL/MySQL Connector C++"
        "$ENV{USERPROFILE}/MySQL/MySQL Connector C++"
    )
else()
    set(MYSQLCPPCONN_PATHS
        /usr/local
        /usr
        # Additional paths
        /opt
        /opt/mysql-connector-c++
        $ENV{HOME}/.local
        $ENV{HOME}/.local/opt/mysql-connector-c++
    )
endif()

# Add environment variable path if set
if(DEFINED ENV{MYSQLCPPCONN_DIR})
    list(APPEND MYSQLCPPCONN_PATHS "$ENV{MYSQLCPPCONN_DIR}")
endif()

# Find MySQL Connector/C++
find_library(MYSQLCPPCONN_LIBRARY 
    NAMES mysqlcppconnx libmysqlcppconnx mysqlcppconn libmysqlcppconn
    HINTS ${MYSQLCPPCONN_PATHS}
    PATH_SUFFIXES lib lib64 lib/x86_64-linux-gnu lib/x86_64-linux-gnu/mysql
)

find_path(MYSQLCPPCONN_INCLUDE_DIR 
    NAMES mysqlx/xdevapi.h
    HINTS ${MYSQLCPPCONN_PATHS}
    PATH_SUFFIXES include include/mysql-connector-c++ include/mysql-connector-c++/mysqlx
)

if(NOT MYSQLCPPCONN_LIBRARY)
    message(FATAL_ERROR "MySQL Connector/C++ library not found. Please install it first.
    You can set MYSQLCPPCONN_DIR environment variable to specify the installation directory.")
endif()

if(NOT MYSQLCPPCONN_INCLUDE_DIR)
    message(FATAL_ERROR "MySQL Connector/C++ headers not found. Please install it first.
    You can set MYSQLCPPCONN_DIR environment variable to specify the installation directory.")
endif()

# Find wxWidgets with improved search paths
if(APPLE)
    set(wxWidgets_SEARCH_PATHS
        /usr/local/lib
        /usr/local/include
        /opt/homebrew/lib
        /opt/homebrew/include
        $ENV{HOME}/homebrew/lib
        $ENV{HOME}/homebrew/include
    )
    if(HOMEBREW_PREFIX)
        list(APPEND wxWidgets_SEARCH_PATHS "${HOMEBREW_PREFIX}/lib" "${HOMEBREW_PREFIX}/include")
    endif()
elseif(WIN32)
    set(wxWidgets_SEARCH_PATHS
        "C:/wxWidgets"
        "$ENV{ProgramFiles}/wxWidgets"
        "$ENV{ProgramFiles(x86)}/wxWidgets"
        "$ENV{LOCALAPPDATA}/wxWidgets"
    )
else()
    set(wxWidgets_SEARCH_PATHS
        /usr/local/lib
        /usr/local/include
        /usr/lib
        /usr/include
        /opt/wxWidgets/lib
        /opt/wxWidgets/include
    )
endif()

find_package(wxWidgets REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

# GTest configuration
option(BUILD_TESTS "Build tests" OFF)

# Source files
set(SOURCE_DIRS
    core
    database
    repositories
    services
    utils
    ui
)

foreach(DIR ${SOURCE_DIRS})
    string(TOUPPER ${DIR} DIR_UPPER)
    file(GLOB_RECURSE ${DIR_UPPER}_SOURCES ${CMAKE_SOURCE_DIR}/src/${DIR}/*.cpp)
endforeach()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${MYSQLCPPCONN_INCLUDE_DIR}
)

# Create libraries
add_library(utils_lib STATIC ${UTILS_SOURCES})

add_library(database_lib STATIC ${DATABASE_SOURCES})
target_link_libraries(database_lib PRIVATE ${MYSQLCPPCONN_LIBRARY} utils_lib)

add_library(core_lib STATIC ${CORE_SOURCES})
target_link_libraries(core_lib PRIVATE utils_lib)

add_library(repository_lib STATIC ${REPOSITORIES_SOURCES})
target_link_libraries(repository_lib PRIVATE core_lib database_lib utils_lib)

add_library(services_lib STATIC ${SERVICES_SOURCES})
target_link_libraries(services_lib PRIVATE repository_lib core_lib database_lib utils_lib)

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp ${UI_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${wxWidgets_INCLUDE_DIRS}
    ${MYSQLCPPCONN_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
    services_lib
    repository_lib
    core_lib
    database_lib
    utils_lib
    ${MYSQLCPPCONN_LIBRARY}
    ${wxWidgets_LIBRARIES}
)

# Test configuration
if(BUILD_TESTS)
    # Find GTest package
    if(APPLE)
        set(GTEST_SEARCH_PATHS
            /usr/local/lib
            /usr/local/include
            /opt/homebrew/lib
            /opt/homebrew/include
            $ENV{HOME}/homebrew/lib
            $ENV{HOME}/homebrew/include
        )
        if(HOMEBREW_PREFIX)
            list(APPEND GTEST_SEARCH_PATHS "${HOMEBREW_PREFIX}/lib" "${HOMEBREW_PREFIX}/include")
        endif()
    elseif(WIN32)
        set(GTEST_SEARCH_PATHS
            "C:/googletest"
            "$ENV{ProgramFiles}/googletest"
            "$ENV{ProgramFiles(x86)}/googletest"
            "$ENV{LOCALAPPDATA}/googletest"
        )
    else()
        set(GTEST_SEARCH_PATHS
            /usr/local/lib
            /usr/local/include
            /usr/lib
            /usr/include
            /opt/googletest/lib
            /opt/googletest/include
        )
    endif()

    # Try to find GTest package first
    find_package(GTest QUIET)
    
    if(NOT GTest_FOUND)
        # If not found, try to find the library manually
        find_library(GTEST_LIBRARY
            NAMES gtest libgtest
            HINTS ${GTEST_SEARCH_PATHS}
            PATH_SUFFIXES lib lib64
        )
        find_library(GTEST_MAIN_LIBRARY
            NAMES gtest_main libgtest_main
            HINTS ${GTEST_SEARCH_PATHS}
            PATH_SUFFIXES lib lib64
        )
        find_path(GTEST_INCLUDE_DIR
            NAMES gtest/gtest.h
            HINTS ${GTEST_SEARCH_PATHS}
            PATH_SUFFIXES include
        )

        if(NOT GTEST_LIBRARY OR NOT GTEST_MAIN_LIBRARY OR NOT GTEST_INCLUDE_DIR)
            message(FATAL_ERROR "Google Test not found. Please install it first.
            You can install it using:
            - macOS: brew install googletest
            - Windows: vcpkg install gtest
            - Linux: sudo apt-get install libgtest-dev")
        endif()

        # Create imported targets for GTest
        add_library(GTest::gtest UNKNOWN IMPORTED)
        set_target_properties(GTest::gtest PROPERTIES
            IMPORTED_LOCATION "${GTEST_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIR}"
        )

        add_library(GTest::gtest_main UNKNOWN IMPORTED)
        set_target_properties(GTest::gtest_main PROPERTIES
            IMPORTED_LOCATION "${GTEST_MAIN_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${GTEST_INCLUDE_DIR}"
        )
    endif()

    # Enable testing
    enable_testing()

    # Find all test files
    file(GLOB_RECURSE TEST_SOURCES 
        "${CMAKE_SOURCE_DIR}/src/tests/*.cpp"
    )

    # Create a list of test executables
    set(TEST_EXECUTABLES)

    # Process each test file
    foreach(TEST_SOURCE ${TEST_SOURCES})
        # Get the test name from the file path
        get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
        
        # Create executable for this test
        add_executable(${TEST_NAME} ${TEST_SOURCE})
        
        # Add to list of test executables
        list(APPEND TEST_EXECUTABLES ${TEST_NAME})
        
        # Link with all necessary libraries
        target_link_libraries(${TEST_NAME} PRIVATE
            GTest::gtest_main
            pthread
            services_lib
            repository_lib
            core_lib
            database_lib
            utils_lib
            ${MYSQLCPPCONN_LIBRARY}
        )
        
        # Add test to CTest
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    endforeach()

    # Print found test files
    message(STATUS "Found test files:")
    foreach(TEST_SOURCE ${TEST_SOURCES})
        message(STATUS "  ${TEST_SOURCE}")
    endforeach()
endif()