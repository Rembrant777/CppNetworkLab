cmake_minimum_required(VERSION 3.10)
project(MuduoEcho)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Get the installation prefix from the environment or use a default value
if(NOT DEFINED INSTALL_DIR)
    set(INSTALL_DIR "/usr/local")  # Default installation path
endif()

# Specify the include directories for Muduo
include_directories(${SOURCE_DIR}/../muduo/include)

# Specify the Muduo libraries location
link_directories(${SOURCE_DIR}/../muduo/lib)

# Find and link the necessary libraries
find_package(Boost REQUIRED)   # If Boost is needed
find_package(Threads REQUIRED)

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Add the executable targets
add_executable(echo_server echo_server.cc)
add_executable(echo_client echo_client.cc)

# Link the Muduo libraries
target_link_libraries(echo_server muduo_net muduo_base pthread)
target_link_libraries(echo_client muduo_net muduo_base pthread)

# Add test executable for integration tests
add_executable(test_integration test_integration.cc)
target_link_libraries(test_integration ${GTEST_LIBRARIES} pthread muduo_net muduo_base)

# Add test executable for performance tests
add_executable(test_performance test_performance.cc)
target_link_libraries(test_performance ${GTEST_LIBRARIES} pthread muduo_net muduo_base)

# Specify the installation rules
install(TARGETS echo_server echo_client test_integration test_performance
        RUNTIME DESTINATION bin)
