#!/bin/sh

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-../build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-/usr/local}  # Modify to system path
CXX=${CXX:-g++}

# Create symbolic link for compile_commands.json
ln -sf $BUILD_DIR/$BUILD_TYPE-cpp11/compile_commands.json

# Create the build directory and navigate to it
mkdir -p $BUILD_DIR/$BUILD_TYPE-cpp11 \
  && cd $BUILD_DIR/$BUILD_TYPE-cpp11 \
  && cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           $SOURCE_DIR \
  && make $* \
  && make install  # Install Google Test

# Optionally run all the unit tests
# CTEST_OUTPUT_ON_FAILURE=TRUE make test

# Generate Doxygen documentation if needed
# cd $SOURCE_DIR && doxygen

