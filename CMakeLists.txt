cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(AI_SLOP VERSION 1.0.0 LANGUAGES CXX)

# Enable Qt AUTOMOC, AUTORCC, and AUTOUIC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Find Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Check if OpenCV was found
if(NOT OpenCV_FOUND)
    message(FATAL_ERROR "OpenCV library not found. Please install OpenCV 4.x")
endif()

message(STATUS "OpenCV version: ${OpenCV_VERSION}")
message(STATUS "OpenCV libraries: ${OpenCV_LIBS}")

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/cli
    ${CMAKE_SOURCE_DIR}/src/advanced
    ${CMAKE_SOURCE_DIR}/src/gui
    ${OpenCV_INCLUDE_DIRS}
    /opt/homebrew/include/opencv4
    /opt/homebrew/Cellar/opencv/4.11.0_1/include/opencv4
    ${Qt5Widgets_INCLUDE_DIRS}
)

# All source files for the main application
set(SOURCES
    src/main.cpp
    src/cli/menu_interface.cpp
    src/advanced/object_detection.cpp
    src/advanced/face_detection.cpp
    src/advanced/video_processing.cpp
    src/advanced/inpainting.cpp
    src/core/brightness_processor.cpp
    src/core/morphology_processor.cpp
    src/core/resize_processor.cpp
    src/core/canny_processor.cpp
    src/core/stitch_processor.cpp
    src/gui/main_window.cpp
)

# Add the main executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link against OpenCV for the main executable
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# Add the test executables
add_executable(test_canny tests/test_canny.cpp src/core/brightness_processor.cpp src/core/morphology_processor.cpp src/core/resize_processor.cpp src/core/canny_processor.cpp src/core/stitch_processor.cpp src/core/image_processor.cpp)
add_executable(test_face_detection tests/test_face_detection.cpp src/advanced/object_detection.cpp src/advanced/face_detection.cpp src/advanced/video_processing.cpp src/advanced/inpainting.cpp)
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

# Add the Qt GUI sources
set(QT_GUI_SOURCES
    src/gui/qt_mainwindow.cpp
    src/gui/main_gui.cpp
    src/gui/qt_mainwindow.hpp
    src/core/brightness_processor.cpp
    src/core/morphology_processor.cpp
    src/core/resize_processor.cpp
    src/core/canny_processor.cpp
    src/core/stitch_processor.cpp
    src/core/image_processor.cpp
    src/advanced/object_detection.cpp
    src/advanced/face_detection.cpp
    src/advanced/video_processing.cpp
    src/advanced/inpainting.cpp
)

# Add the Qt GUI executable
add_executable(AI_SLOP_GUI ${QT_GUI_SOURCES})
target_link_libraries(AI_SLOP_GUI Qt5::Widgets ${OpenCV_LIBS})

# Copy model files to build directory
file(COPY ${CMAKE_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/images DESTINATION ${CMAKE_BINARY_DIR})

# Output message
message(STATUS "CMake configuration finished for ${PROJECT_NAME}") 