# OKSI Backend Image Streaming Protocol Library

This contains image streaming backend code for the SOMAV project.  It uses ZeroMQ and boost serialization as dependencies.

## Installation Dependencies

sudo ./INSTALL

## Build

mkdir build
cd build
cmake ..
make -j8

## Build With Tests

Make sure you have OpenCV3+ installed to build the tests.  Refer to the OpenCV website for instructions.

mkdir build
cd build
cmake -DWITH_TESTS=ON ..
make -j8

## Submodule

If running as a submodule make sure to reference the lib directory to link against libproto.  
Be sure to have libproto up to date by running:

git submodule init && git submodule update
