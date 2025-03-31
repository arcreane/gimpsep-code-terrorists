#ifndef AI_SLOP_CLI_PARSER_HPP
#define AI_SLOP_CLI_PARSER_HPP

#include <string>
#include <vector>
#include <optional> // Used for optional arguments
#include <iostream> // For error messages

// Include the cxxopts header
#include "cxxopts.hpp"

// Forward declaration from OpenCV (to avoid including heavy headers here if possible)
// However, for simplicity in this header, direct inclusion might be okay,
// but let's try to keep dependencies minimal if we just need types.
// For now, we'll stick to standard types and handle cv::Mat elsewhere.

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
inline ParsedArguments parse_arguments(int argc, char** argv) {
    ParsedArguments args;
    try {
        cxxopts::Options options(argv[0], "AI_SLOP - Multimedia Image Processing Application");

        options.add_options()
            ("h,help", "Display this help message")
            ("op,operation", "The operation to perform (dilate, erode, resize, brightness, stitch, canny)", cxxopts::value<std::string>())
            ("i,input", "Input image file path(s). Multiple allowed for stitch.", cxxopts::value<std::vector<std::string>>())
            ("o,output", "Output image file path", cxxopts::value<std::string>())
            // Operation-specific options
            ("k,kernel_size", "Kernel size for dilation/erosion (positive odd integer)", cxxopts::value<int>()->default_value("3"))
            ("f,factor", "Resize factor (e.g., 1.5 for 150%, 0.5 for 50%)", cxxopts::value<double>())
            ("b,brightness", "Value to add/subtract for brightness adjustment (-255 to 255)", cxxopts::value<int>()->default_value("0"))
            ("t1,threshold1", "First threshold for the Canny edge detector hysteresis procedure", cxxopts::value<double>()->default_value("100.0"))
            ("t2,threshold2", "Second threshold for the Canny edge detector hysteresis procedure", cxxopts::value<double>()->default_value("200.0"));

        // Allow input files to be positional for convenience (e.g., ./AI_SLOP --op stitch img1.jpg img2.jpg -o out.jpg)
        // options.parse_positional("input"); // Let's stick to explicit -i for now for clarity

        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            args.show_help = true;
            // Print help message directly here for immediate feedback
            std::cout << options.help() << std::endl;
            return args; // Return immediately if help is requested
        }

        // --- Argument Validation ---

        // Operation is mandatory unless help is requested
        if (!result.count("operation")) {
            throw std::runtime_error("Operation (--operation or --op) is required.");
        }
        args.operation = result["operation"].as<std::string>();

        // Input file(s) are mandatory for all operations
        if (!result.count("input")) {
            throw std::runtime_error("Input file path(s) (--input or -i) are required.");
        }
        args.input_files = result["input"].as<std::vector<std::string>>();

        // Output file is mandatory for all operations
        if (!result.count("output")) {
            throw std::runtime_error("Output file path (--output or -o) is required.");
        }
        args.output_file = result["output"].as<std::string>();

        // --- Populate Optional Parameters ---

        // Dilate/Erode specific
        if (args.operation == "dilate" || args.operation == "erode") {
            args.kernel_size = result["kernel_size"].as<int>();
            // Basic validation for kernel size
            if (args.kernel_size.value() <= 0 || args.kernel_size.value() % 2 == 0) {
                throw std::runtime_error("Kernel size (--kernel_size or -k) must be a positive odd integer.");
            }
        }

        // Resize specific
        if (args.operation == "resize") {
            if (!result.count("factor")) {
                 throw std::runtime_error("Resize factor (--factor or -f) is required for resize operation.");
            }
            args.resize_factor = result["factor"].as<double>();
             if (args.resize_factor.value() <= 0) {
                throw std::runtime_error("Resize factor (--factor or -f) must be positive.");
            }
        }

        // Brightness specific
        if (args.operation == "brightness") {
             // Use default if not provided, but capture it
             args.brightness_value = result["brightness"].as<int>();
             // Optional: Validate range, though OpenCV might clamp it
             // if (args.brightness_value.value() < -255 || args.brightness_value.value() > 255) {
             //    throw std::runtime_error("Brightness value must be between -255 and 255.");
             // }
        }

        // Canny specific
        if (args.operation == "canny") {
            args.canny_threshold1 = result["threshold1"].as<double>();
            args.canny_threshold2 = result["threshold2"].as<double>();
            if (args.canny_threshold1.value() < 0 || args.canny_threshold2.value() < 0) {
                 throw std::runtime_error("Canny thresholds must be non-negative.");
            }
             if (args.canny_threshold1.value() > args.canny_threshold2.value()) {
                 // OpenCV documentation often suggests threshold2 > threshold1
                 std::cerr << "Warning: threshold1 is greater than threshold2 for Canny detector." << std::endl;
             }
        }

        // Stitch specific validation
        if (args.operation == "stitch" && args.input_files.size() < 2) {
            throw std::runtime_error("Stitch operation requires at least two input images.");
        }


    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        // Re-throw as a standard exception or handle differently
        throw std::runtime_error("Argument parsing failed.");
    } catch (const std::runtime_error& e) {
        std::cerr << "Argument Error: " << e.what() << std::endl;
        throw; // Re-throw validation errors
    }

    return args;
}

#endif // AI_SLOP_CLI_PARSER_HPP 