cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(AI_SLOP VERSION 1.0)

# Set the C++ standard (e.g., C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- OpenCV Configuration ---
# Set the path to your OpenCV installation directory.
# IMPORTANT: Replace this path with the correct one for your system if CMake doesn't find OpenCV automatically.
# This is the directory containing 'OpenCVConfig.cmake' or 'opencv-config.cmake'.
set(OpenCV_DIR "C:/OpenCV-MinGW-Build-OpenCV-4.5.5-x64") # Adjusted based on your PATH

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Check if OpenCV was found
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV library not found. Please set OpenCV_DIR correctly in CMakeLists.txt")
endif()

message(STATUS "OpenCV libraries found: ${OpenCV_LIBS}")

# Include directories (for OpenCV headers)
include_directories(${OpenCV_INCLUDE_DIRS})

# --- Executable Definition ---

# Add the executable target
add_executable(${PROJECT_NAME} src/main.cpp)

# Link the executable against the OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# --- Optional: Installation ---
# (You can add installation rules here later if needed)
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# --- Output ---
message(STATUS "CMake configuration finished for ${PROJECT_NAME}") 