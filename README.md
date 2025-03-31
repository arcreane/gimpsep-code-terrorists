# AI_SLOP: Multimedia Image Processing Application

A command-line application for performing various image processing tasks using OpenCV in C++.

## Features

*   (Planned) Image Dilation/Erosion
*   (Planned) Image Resizing
*   (Planned) Brightness Adjustment
*   (Planned) Image Stitching (Panorama)
*   (Planned) Canny Edge Detection
*   (Planned) Advanced Features (TBD)

## Prerequisites

*   C++ Compiler (supporting C++17 or later)
*   CMake (version 3.10 or later)
*   OpenCV (version 4.x recommended)

## Building

*(Instructions TBD)*

## Usage

*   **`stitch`**: Stitches multiple images into a panorama.
    *   Requires: `--input <image1_path> <image2_path> ...`, `--output <image_path>`
    *   Example: `./AI_SLOP --op stitch -i pano1.jpg -i pano2.jpg -i pano3.jpg -o panorama.jpg` (Note: Use multiple `-i` flags for input files)
*   **`canny`**: Detects edges using the Canny algorithm.
    *   Requires: `--input <image_path>`, `--output <image_path>`
    *   Options: `--threshold1 <float>` (default: 100.0), `--threshold2 <float>` (default: 200.0) (Note: Use full names, not `-t1`/`-t2`)
    *   Example: `./AI_SLOP --op canny -i input.jpg -o edges.jpg --threshold1 50 --threshold2 150`
*   **`video-gray`**: Converts an input video to grayscale.
*   **`help`**: Displays help information.
    *   Example: `./AI_SLOP --help` 