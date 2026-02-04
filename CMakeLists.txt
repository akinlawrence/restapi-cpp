

cmake_minimum_required(VERSION 3.10)

project(restapi VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect all source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
set(MAIN_FILE "${PROJECT_SOURCE_DIR}/main.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${MAIN_FILE} ${SOURCES})

# Add include folder for this target
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")

include(CTest)
enable_testing()

# Optional: compiler warnings
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()