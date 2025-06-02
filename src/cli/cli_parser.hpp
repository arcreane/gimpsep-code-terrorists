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
inline ParsedArguments parse_arguments(int argc, char** argv) {
    ParsedArguments args;
    try {
        cxxopts::Options options(argv[0], "AI_SLOP - Multimedia Image Processing Application");

        options.add_options()
            ("h,help", "Display this help message")
            ("op,operation", "The operation to perform (dilate, erode, resize, brightness, stitch, canny, video-gray, detect-faces, bg-subtract, detect-objects, inpaint)", cxxopts::value<std::string>())
            ("i,input", "Input image/video file path(s). Multiple allowed for stitch.", cxxopts::value<std::vector<std::string>>())
            ("o,output", "Output image/video file path", cxxopts::value<std::string>())
            // Core operation-specific options
            ("k,kernel_size", "Kernel size for dilation/erosion (positive odd integer)", cxxopts::value<int>()->default_value("3"))
            ("f,factor", "Resize factor (e.g., 1.5 for 150%, 0.5 for 50%)", cxxopts::value<double>())
            ("b,brightness", "Value to add/subtract for brightness adjustment (-255 to 255)", cxxopts::value<int>()->default_value("0"))
            ("t1,threshold1", "First threshold for the Canny edge detector hysteresis procedure", cxxopts::value<double>()->default_value("100.0"))
            ("t2,threshold2", "Second threshold for the Canny edge detector hysteresis procedure", cxxopts::value<double>()->default_value("200.0"))
            // Advanced operation-specific options
            ("c,cascade", "Path to the cascade classifier XML file (for detect-faces)", cxxopts::value<std::string>())
            // YOLO Object Detection options
            ("yolo_cfg", "Path to YOLO .cfg file (for detect-objects)", cxxopts::value<std::string>())
            ("yolo_weights", "Path to YOLO .weights file (for detect-objects)", cxxopts::value<std::string>())
            ("yolo_names", "Path to YOLO .names file (for detect-objects)", cxxopts::value<std::string>())
            ("conf", "Confidence threshold (for detect-objects)", cxxopts::value<float>()->default_value("0.5"))
            ("nms", "NMS threshold (for detect-objects)", cxxopts::value<float>()->default_value("0.4"))
            // Inpainting options
            ("m,mask", "Path to the mask image (for inpaint)", cxxopts::value<std::string>())
            ("radius", "Inpainting radius (for inpaint)", cxxopts::value<double>()->default_value("3.0"))
            ("inpaint_method", "Inpainting method: NS or TELEA (for inpaint)", cxxopts::value<std::string>()->default_value("NS"));

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
        args.kernel_size = result["kernel_size"].as<int>(); // Always parse, default exists
        args.brightness_value = result["brightness"].as<int>(); // Always parse, default exists
        args.canny_threshold1 = result["threshold1"].as<double>(); // Always parse, default exists
        args.canny_threshold2 = result["threshold2"].as<double>(); // Always parse, default exists

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

        // Video specific validation (expects exactly one input)
        if ((args.operation == "video-gray" || args.operation == "bg-subtract") 
             && args.input_files.size() != 1) {
            throw std::runtime_error("Video operations (video-gray, bg-subtract) require exactly one input video file.");
        }

        // Face Detection specific
        if (args.operation == "detect-faces") {
            if (!result.count("cascade")) {
                 throw std::runtime_error("Cascade file path (--cascade or -c) is required for detect-faces operation.");
            }
            args.cascade_file = result["cascade"].as<std::string>();
            // Could add validation here to check if the file string is non-empty
        }

        // Object Detection specific
        if (args.operation == "detect-objects") {
            if (!result.count("yolo_cfg") || !result.count("yolo_weights") || !result.count("yolo_names")) {
                 throw std::runtime_error("YOLO model files (--yolo_cfg, --yolo_weights, --yolo_names) are required for detect-objects operation.");
            }
            args.yolo_config = result["yolo_cfg"].as<std::string>();
            args.yolo_weights = result["yolo_weights"].as<std::string>();
            args.yolo_names = result["yolo_names"].as<std::string>();
            args.yolo_conf = result["conf"].as<float>(); // Use default if not provided
            args.yolo_nms = result["nms"].as<float>();   // Use default if not provided

             if (args.yolo_conf <= 0 || args.yolo_conf > 1.0) {
                 throw std::runtime_error("Confidence threshold (--conf) must be between 0 and 1.");
             }
             if (args.yolo_nms <= 0 || args.yolo_nms > 1.0) {
                 throw std::runtime_error("NMS threshold (--nms) must be between 0 and 1.");
             }
        }

        // Inpainting specific
        if (args.operation == "inpaint") {
            if (!result.count("mask")) {
                 throw std::runtime_error("Mask file path (--mask or -m) is required for inpaint operation.");
            }
            args.mask_file = result["mask"].as<std::string>();
            args.inpaint_radius = result["radius"].as<double>(); // Use default if not provided
            args.inpaint_method = result["inpaint_method"].as<std::string>(); // Use default if not provided

            if (args.inpaint_radius.value() <= 0) {
                throw std::runtime_error("Inpaint radius (--radius) must be positive.");
            }
            std::string method_upper = args.inpaint_method.value();
            // Convert method to upper case for comparison
            std::transform(method_upper.begin(), method_upper.end(), method_upper.begin(), ::toupper);
            if (method_upper != "NS" && method_upper != "TELEA") {
                 throw std::runtime_error("Invalid inpainting method (--inpaint_method). Must be NS or TELEA.");
            }
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