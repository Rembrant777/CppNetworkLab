cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BlockchainRPC)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find required packages
find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)
#find_package(gRPC REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS} ../../core)

# List of test source files
set(TEST_SOURCES
    blockchain_rpc_client_test.cpp
    blockchain_rpc_server_test.cpp
    integration_test.cpp
    perf_test.cpp
)

# Loop through each test source and create an executable for it
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Get the filename without extension for the executable name
    get_filename_component(EXECUTABLE_NAME ${TEST_SOURCE} NAME_WE)

    # Create the executable for this test source
    add_executable(${EXECUTABLE_NAME} ${TEST_SOURCE})

    # Link libraries for the test executable
    target_link_libraries(${EXECUTABLE_NAME} GTest::GTest GTest::Main ${Protobuf_LIBRARIES})

    # Enable C++ exceptions for GTest
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )
endforeach()