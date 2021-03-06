project(protocol)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0")
SET(CMAKE_C_FLAGS_DEBUG "-O0")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

set(CMAKE_BUILD_TYPE Debug)

# OpenCV
find_package(OpenCV 3.0.0 REQUIRED)

# Boost
find_package(Boost 1.54 COMPONENTS iostreams serialization)

# Include directories
include_directories(
	/usr/include
	/usr/local/include/
	${PROJECT_SOURCE_DIR}/include
)

link_directories(
	/usr/lib
	/usr/local/lib
	${PROJECT_SOURCE_DIR}/lib
)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADER "${PROJECT_SOURCE_DIR}/include/*.h*")

add_library(proto STATIC ${SOURCES} ${HEADERS})

# Build tests
add_executable(testServer ${PROJECT_SOURCE_DIR}/test/testServer.cpp)
target_link_libraries(testServer proto ${OpenCV_LIBS} z boost_serialization)
add_executable(testClient ${PROJECT_SOURCE_DIR}/test/testClient.cpp)
target_link_libraries(testClient proto ${OpenCV_LIBS} z boost_serialization)
add_executable(testSerialization ${PROJECT_SOURCE_DIR}/test/testSerialization.cpp)
target_link_libraries(testSerialization proto ${OpenCV_LIBS} z boost_serialization)