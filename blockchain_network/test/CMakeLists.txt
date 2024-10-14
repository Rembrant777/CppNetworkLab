cmake_minimum_required(VERSION 3.10)
project(BlockchainNetwork)

# C++ LIB standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# sub-folders 
add_subdirectory(core)
add_subdirectory(echo)

# append other submodules
# add_subdirectory(another_module)
