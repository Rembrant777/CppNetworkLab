cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BlockchainCoreTest)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# Find required packages
find_package(GTest REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Threads REQUIRED)

# Get the installation prefix from the environment or use a default value
if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR "/usr/local")  # Default installation path
endif()

# Specify the include directories for Muduo
include_directories(../../../third-party/muduo/src) 

# Include core header files 
include_directories(${CMAKE_SOURCE_DIR}/../../core)

# Specify the Muduo libraries location
link_directories(../../../third-party/build/muduo-release-cpp11/lib)

# Find and link the necessary libraries
find_package(Boost REQUIRED)  
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${GTEST_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS} ../../core)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../core)

# List of core source files 
set(CORE_SOURCES 
    ../../../core/blockchain.cpp
    ../../../core/blockchain_rpc.pb.cc 
    ../../../core/blockchain_rpc_client.cpp 
    ../../../core/blockchain_rpc_server.cpp
)

# List of test source files
set(TEST_SOURCES
    blockchain_rpc_client_test.cpp
    blockchain_rpc_server_test.cpp
    integration_test.cpp
    perf_test.cpp
)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH} /path/to/muduo")

# Loop through each test source and create an executable for it
foreach(TEST_SOURCE ${TEST_SOURCES})
    # Get the filename without extension for the executable name
    get_filename_component(EXECUTABLE_NAME ${TEST_SOURCE} NAME_WE)

    # Create the executable for this test source
    add_executable(${EXECUTABLE_NAME} ${TEST_SOURCE} ${CORE_SOURCES})

    # Link libraries for the test executable
    target_link_libraries(${EXECUTABLE_NAME}
        GTest::GTest 
        GTest::Main 
        ${Protobuf_LIBRARIES}
        Threads::Threads
        muduo_net 
        muduo_base 
        OpenSSL::SSL
        OpenSSL::Crypto
    )

    # Enable C++ exceptions for GTest
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )
endforeach()