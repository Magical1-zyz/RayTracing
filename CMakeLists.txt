cmake_minimum_required(VERSION 3.25)
project(RayTracingInOneWeekend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS -static)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# EXECUTABLE_OUTPUT_PATH : output path of target binary file
# PROJECT_SOURCE_DIR : Project root directory.
# Set the path where the binary file is stored to the bin directory in the project root directory

# Store all source files under the directory . /src into the variable SRC_LIST.
aux_source_directory(./src SRC_LIST)
# You can also use the set command to create a new variable to store the source file.
# set(SRC_LIST ./src/main.cpp ./src/hittable_list.cpp)

# Specify the search path for the header file
include_directories(./include)

# Generate the target binary file
add_executable(RayTracingInOneWeekend ${SRC_LIST} )

