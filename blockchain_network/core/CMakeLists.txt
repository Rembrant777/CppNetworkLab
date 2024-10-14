cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BlockchainCore)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find required packages
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories("/root/WorkSpace/CppNetworkLab/blockchain_network/third-party/muduo/src")

link_directories("/root/WorkSpace/CppNetworkLab/blockchain_network/third-party/build/muduo-release-cpp11/lib")

# List of source files
set(SOURCES
    blockchain.cpp
    blockchain_rpc_client.cpp
    blockchain_rpc_server.cpp
    blockchain_rpc.pb.cc  # Protobuf generated source
)

# Link libraries
add_library(blockchain_lib ${SOURCES})

target_link_libraries(blockchain_lib ${Protobuf_LIBRARIES} muduo_net muduo_base OpenSSL::SSL OpenSSL::Crypto)