cmake_minimum_required(VERSION 3.25)
project(RayTracingInOneWeekend)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS -static)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
# CMake_RUNTIME_OUTPUT_DIRECTORYU : output path of target binary file
# PROJECT_SOURCE_DIR : Project root directory.
# Set the path where the binary file is stored to the bin directory in the project root directory

# Store all source files under the directory . /src into the variable SRC_LIST.
#aux_source_directory(./src SRC_LIST)
file(GLOB SRC_LIST "${PROJECT_SOURCE_DIR}/src/main.cpp")
# You can also use the set command to create a new variable to store the source file.
# set(SRC_LIST ./src/main.cpp ./src/hittable_list.cpp)

# Specify the search path for the header file
include_directories("${PROJECT_SOURCE_DIR}/include")

# Generate the target binary file
add_executable(RayTracingInOneWeekend ${SRC_LIST} )
add_executable(pi ${PROJECT_SOURCE_DIR}/src/pi.cpp)
add_executable(estimate_halfway ${PROJECT_SOURCE_DIR}/src/estimate_halfway.cpp)
add_executable(integrate_x_sq ${PROJECT_SOURCE_DIR}/src/integrate_x_sq.cpp)
add_executable(sphere_importance ${PROJECT_SOURCE_DIR}/src/sphere_importance.cpp)
add_executable(sphere_plot ${PROJECT_SOURCE_DIR}/src/sphere_plot.cpp)
add_executable(cos_cubed ${PROJECT_SOURCE_DIR}/src/cos_cubed.cpp)
add_executable(cos_density ${PROJECT_SOURCE_DIR}/src/cos_density.cpp)


