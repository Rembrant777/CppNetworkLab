cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BlockchainCore)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 11)

# Find required packages
find_package(Protobuf REQUIRED)
#find_package(gRPC REQUIRED)

# Include directories
include_directories(${Protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS})

# List of source files
set(SOURCES
    blockchain.cpp
    blockchain_rpc_client.cpp
    blockchain_rpc_server.cpp
    blockchain_rpc.pb.cc  # Protobuf generated source
)

# Add the executable (if needed for building server or client)
# If you're building standalone binaries:
# add_executable(blockchain_server blockchain_rpc_server.cpp blockchain_rpc.pb.cc blockchain.cpp)
# add_executable(blockchain_client blockchain_rpc_client.cpp blockchain_rpc.pb.cc blockchain.cpp)

# Link libraries
add_library(blockchain_lib ${SOURCES})

target_link_libraries(blockchain_lib ${Protobuf_LIBRARIES})

# Generate protobuf and gRPC files from .proto
#set(PROTO_FILES blockchain_rpc.proto)

#set(GENERATED_FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/blockchain_rpc.pb.cc
#    ${CMAKE_CURRENT_BINARY_DIR}/blockchain_rpc.pb.h)

# Add custom commands to generate .cc and .h files from .proto
#add_custom_command(
#    OUTPUT ${GENERATED_FILES}
#    COMMAND ${Protobuf_PROTOC_EXECUTABLE} --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` --cpp_out=. ${PROTO_FILES}
#    DEPENDS ${PROTO_FILES}
#)

# Ensure protobuf-generated files are built before compiling other sources
#add_custom_target(generate_proto_files ALL DEPENDS ${GENERATED_FILES})

# Include directory for generated files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Add the generated .cc files to the build
#add_library(blockchain_rpc_proto ${GENERATED_FILES})
#target_link_libraries(blockchain_rpc_proto ${Protobuf_LIBRARIES})

# You can link this library to other executables like:
# target_link_libraries(blockchain_server blockchain_lib blockchain_rpc_proto)
# target_link_libraries(blockchain_client blockchain_lib blockchain_rpc_proto)