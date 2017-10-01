project(protocol)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11 -g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

# OpenCV
find_package(OpenCV 3.0.0 REQUIRED)

# Boost
find_package(Boost 1.54 COMPONENTS iostreams serialization)

# Include directories
include_directories(
    /user/include
    /usr/local/include/
	./zmq/include
    ${PROJECT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

link_directories(
	/usr/lib
	/usr/local/lib
)

file(GLOB SOURCES "*.cpp")
file(GLOB HEADER "*.h*")

add_library(libProto STATIC ${SOURCES} ${HEADERS})

add_executable(testProtocol testProtocol.cpp)
target_link_libraries(testProtocol libProto ${OpenCV_LIBS} zmq boost_system boost_iostreams boost_serialization pthread)
