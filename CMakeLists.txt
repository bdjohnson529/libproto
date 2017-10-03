project(protocol)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")

# OpenCV
#find_package(OpenCV 3.0.0 REQUIRED)

# Boost
find_package(Boost 1.54 COMPONENTS iostreams serialization)

# Include directories
include_directories(
    /usr/include
    /usr/local/include/
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/include/zmq
    #${OpenCV_INCLUDE_DIRS}
)

link_directories(
	/usr/lib
	/usr/local/lib
	${PROJECT_SOURCE_DIR}/lib
)

file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADER "${PROJECT_SOURCE_DIR}/include/*.h*")

add_library(proto STATIC ${SOURCES} ${HEADERS})

#add_executable(testProtocol ${PROJECT_SOURCE_DIR}/test/testProtocol.cpp)
#target_link_libraries(testProtocol proto ${OpenCV_LIBS} z zmq boost_system boost_iostreams boost_serialization pthread)
