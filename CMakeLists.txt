cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(AI_SLOP VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Check if OpenCV was found
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV library not found. Please install OpenCV 4.x")
endif()

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/cli
    ${CMAKE_CURRENT_SOURCE_DIR}/src/advanced
    ${OpenCV_INCLUDE_DIRS}
    /opt/homebrew/include/opencv4
)

# Core image processing sources
set(CORE_SOURCES
    src/core/image_processor.cpp
    src/core/brightness_processor.cpp
    src/core/morphology_processor.cpp
    src/core/resize_processor.cpp
    src/core/canny_processor.cpp
    src/core/stitch_processor.cpp
)

# CLI interface sources
set(CLI_SOURCES
    src/cli/cli_parser.cpp
    src/cli/cli_interface.cpp
)

# Advanced processing sources
set(ADVANCED_SOURCES
    src/advanced/video_processing.cpp
    src/advanced/face_detection.cpp
    src/advanced/object_detection.cpp
    src/advanced/inpainting.cpp
)

# Main application source
set(MAIN_SOURCE
    src/main.cpp
)

# Test program sources
set(TEST_SOURCES
    tests/test_canny.cpp
    tests/test_face_detection.cpp
)

# Combine all sources for main application
set(APP_SOURCES
    ${CORE_SOURCES}
    ${CLI_SOURCES}
    ${ADVANCED_SOURCES}
    ${MAIN_SOURCE}
)

# Add the main executable
add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Add the test executables
add_executable(test_canny tests/test_canny.cpp ${CORE_SOURCES})
add_executable(test_face_detection tests/test_face_detection.cpp ${ADVANCED_SOURCES})

# Link against OpenCV for all executables
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
target_link_libraries(test_canny ${OpenCV_LIBS})
target_link_libraries(test_face_detection ${OpenCV_LIBS})

# Enable warnings
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(test_canny PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(test_face_detection PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
    target_compile_options(test_canny PRIVATE /W4)
    target_compile_options(test_face_detection PRIVATE /W4)
endif()

# Installation targets
install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(DIRECTORY src/
        DESTINATION include/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.hpp")

# Output message
message(STATUS "CMake configuration finished for ${PROJECT_NAME}") 