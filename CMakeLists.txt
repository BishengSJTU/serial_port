cmake_minimum_required(VERSION 3.5)
project(serialPort)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp serialport.h serialport.cpp)
add_executable(serialPort ${SOURCE_FILES})
