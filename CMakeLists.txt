cmake_minimum_required(VERSION 3.10)
project(riscv_simulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add sources
file(GLOB SOURCES "src/*.cpp")

# Define executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)
