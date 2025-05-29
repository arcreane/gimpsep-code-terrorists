#pragma once

#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include <iostream> // For error messages

// Include the cxxopts header
#include "cxxopts.hpp"

// Forward declaration from OpenCV (to avoid including heavy headers here if possible)
// However, for simplicity in this header, direct inclusion might be okay,
// but let's try to keep dependencies minimal if we just need types.
// For now, we'll stick to standard types and handle cv::Mat elsewhere.

namespace ai_slop {

/**
 * @brief Structure to hold the parsed command-line arguments.
 */
struct ParsedArguments {
    bool show_help = false;                 // Flag if --help was requested
    std::string operation;                  // The image processing operation requested
    std::vector<std::string> input_files;   // List of input image file paths
    std::string output_file;                // Output image file path

    // Operation-specific parameters (using std::optional for clarity)
    std::optional<int> kernel_size;         // For dilate/erode
    std::optional<double> resize_factor;    // For resize
    std::optional<int> brightness_value;    // For brightness adjustment
    std::optional<double> canny_threshold1; // For Canny edge detection
    std::optional<double> canny_threshold2; // For Canny edge detection

    // --- Advanced Feature Args ---
    std::optional<std::string> cascade_file; // Path to Haar cascade XML for face detection
    std::optional<std::string> yolo_config;  // Path to YOLO config file
    std::optional<std::string> yolo_weights; // Path to YOLO weights file
    std::optional<std::string> yolo_names;   // Path to YOLO class names file
    std::optional<float> yolo_conf;      // YOLO confidence threshold
    std::optional<float> yolo_nms;       // YOLO NMS threshold
    std::optional<std::string> mask_file;  // Path to mask image for inpainting
    std::optional<double> inpaint_radius;// Inpainting radius
    std::optional<std::string> inpaint_method; // Inpainting method (NS or TELEA)

    // Potential future parameters can be added here
};

/**
 * @brief Parses command-line arguments using cxxopts.
 *
 * @param argc The argument count from main().
 * @param argv The argument vector from main().
 * @return ParsedArguments A structure containing the parsed options.
 * @throws std::runtime_error if required arguments are missing or parsing fails.
 */
ParsedArguments parse_arguments(int argc, char** argv);
void print_usage();

} // namespace ai_slop 