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

# Include directories (for OpenCV headers and our source headers)
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src # Include top-level src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cli # Include cli subdirectory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core # Include core subdirectory
    ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced # Include advanced subdirectory
    ${OpenCV_INCLUDE_DIRS}
    )

# --- Executable Definition ---

# List all source files for the executable
set(APP_SOURCES
    src/main.cpp
    src/core/morphology.cpp
    src/core/resize.cpp
    src/core/brightness.cpp
    src/core/stitching.cpp
    src/core/canny.cpp
    src/advanced/video_processing.cpp
    # Add other .cpp files here as they are created
)

# Add the executable target
add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Link the executable against the OpenCV libraries
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# --- Optional: Installation ---
# (You can add installation rules here later if needed)
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# --- Output ---
message(STATUS "CMake configuration finished for ${PROJECT_NAME}") 