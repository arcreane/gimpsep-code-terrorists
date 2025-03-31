#include <iostream>
#include <vector>
#include <string>
#include <stdexcept> // For exception handling

#include <opencv2/core.hpp> // Basic OpenCV structures (cv::Mat)
#include <opencv2/imgcodecs.hpp> // For cv::imread, cv::imwrite
#include <opencv2/highgui.hpp> // For cv::imshow (optional debugging)

// Include the command-line parser header
#include "cli/cli_parser.hpp"

// Include core function headers
#include "core/morphology.hpp"
#include "core/resize.hpp"
#include "core/brightness.hpp"
#include "core/canny.hpp"
#include "core/stitching.hpp"

// Include advanced function headers
#include "advanced/video_processing.hpp"
#include "advanced/face_detection.hpp"

/**
 * @brief Main entry point for the AI_SLOP application.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Exit code (0 for success, 1 for errors).
 */
int main(int argc, char** argv) {
    try {
        // Parse command-line arguments
        ParsedArguments args = parse_arguments(argc, argv);

        // If help was requested, the parser already printed the message and we can exit.
        if (args.show_help) {
            return 0;
        }

        // --- Print Parsed Arguments (for debugging/verification) ---
        std::cout << "--- Parsed Arguments ---" << std::endl;
        std::cout << "Operation: " << args.operation << std::endl;
        std::cout << "Input Files: " << std::endl;
        for (const auto& file : args.input_files) {
            std::cout << "  - " << file << std::endl;
        }
        std::cout << "Output File: " << args.output_file << std::endl;

        // Print operation-specific arguments if they exist
        if (args.kernel_size.has_value()) {
            std::cout << "Kernel Size: " << args.kernel_size.value() << std::endl;
        }
        if (args.resize_factor.has_value()) {
            std::cout << "Resize Factor: " << args.resize_factor.value() << std::endl;
        }
        if (args.brightness_value.has_value()) {
            std::cout << "Brightness Value: " << args.brightness_value.value() << std::endl;
        }
        if (args.canny_threshold1.has_value()) {
            std::cout << "Canny Threshold 1: " << args.canny_threshold1.value() << std::endl;
        }
        if (args.canny_threshold2.has_value()) {
            std::cout << "Canny Threshold 2: " << args.canny_threshold2.value() << std::endl;
        }
        std::cout << "------------------------" << std::endl;


        // --- Image Loading ---
        cv::Mat input_image; // Keep for single-image operations
        // std::vector<cv::Mat> input_images_stitch; // No longer needed as stitch loads internally

        if (args.operation == "stitch") {
            // Stitching loads images internally from paths
             if (args.input_files.size() < 2) {
                 // This check is also in cli_parser, but good to be robust
                 throw std::runtime_error("Stitching operation requires at least two input image paths provided via -i.");
             }
             std::cout << "Stitching operation selected. Image loading will occur in the stitch function." << std::endl;
        }
        else if (args.operation == "video-gray") {
            // Video processing also loads internally from path
            if (args.input_files.size() != 1) { // Validation already in parser, but defensive check
                 throw std::runtime_error("Video operations require exactly one input video path provided via -i.");
             }
             std::cout << "Video operation ('" << args.operation << "') selected. Input video: " << args.input_files[0] << std::endl;
        }
        else if (args.operation == "bg-subtract") {
            // Background subtraction input validation (also video)
            if (args.input_files.size() != 1) { 
                 throw std::runtime_error("Background subtraction requires exactly one input video path.");
             }
            std::cout << "Background subtraction ('" << args.operation << "') selected. Input video: " << args.input_files[0] << std::endl;
        }
        else if (args.operation == "detect-faces") {
            // Standard single image input expected
            if (args.input_files.size() != 1) {
                throw std::runtime_error("Face detection requires exactly one input image path.");
            }
            // Load single image
            input_image = cv::imread(args.input_files[0], cv::IMREAD_COLOR);
            if (input_image.empty()) {
                throw std::runtime_error("Failed to load input image: " + args.input_files[0]);
            }
            std::cout << "Input image loaded: " << args.input_files[0] << std::endl;
        }
        else if (args.input_files.size() == 1) {
            // Load single image for other non-stitch, non-video operations
            input_image = cv::imread(args.input_files[0], cv::IMREAD_COLOR);
            if (input_image.empty()) {
                throw std::runtime_error("Failed to load input image: " + args.input_files[0]);
            }
            std::cout << "Input image loaded: " << args.input_files[0] << std::endl;
        } 
        else {
             // Covers cases like 0 inputs, or >1 input for non-stitch operations
             throw std::runtime_error("Invalid number of input files (" + std::to_string(args.input_files.size()) + ") provided for operation: " + args.operation);
        }

        // --- Function Dispatch --- 
        cv::Mat output_image;
        bool operation_handled = false;

        if (args.operation == "dilate") {
            if (!args.kernel_size.has_value()) {
                // This should be caught by the parser, but double-check
                throw std::runtime_error("Kernel size is required for dilation.");
            }
            std::cout << "Performing dilation..." << std::endl;
            output_image = dilate_image(input_image, args.kernel_size.value());
            operation_handled = true;
        }
        else if (args.operation == "erode") {
            if (!args.kernel_size.has_value()) {
                throw std::runtime_error("Kernel size is required for erosion.");
            }
            std::cout << "Performing erosion..." << std::endl;
            output_image = erode_image(input_image, args.kernel_size.value());
            operation_handled = true;
        }
        else if (args.operation == "resize") {
            if (!args.resize_factor.has_value()) {
                // This should be caught by the parser, but double-check
                throw std::runtime_error("Resize factor (-f or --factor) is required for resize operation.");
            }
            std::cout << "Performing resize..." << std::endl;
            output_image = resize_image(input_image, args.resize_factor.value()); // Using default interpolation (linear)
            operation_handled = true;
        }
        else if (args.operation == "brightness") {
            if (!args.brightness_value.has_value()) {
                // Parser provides a default, so this check might be redundant unless default is removed
                throw std::runtime_error("Brightness value (-b or --brightness) is required for brightness operation.");
            }
            std::cout << "Performing brightness adjustment..." << std::endl;
            output_image = adjust_brightness(input_image, args.brightness_value.value());
            operation_handled = true;
        }
        else if (args.operation == "stitch") {
            std::cout << "Performing stitching..." << std::endl;
            cv::Stitcher::Status status = stitch_images(args.input_files, output_image); // output_image is the pano

            if (status == cv::Stitcher::OK) {
                std::cout << "Stitching completed successfully." << std::endl;
                operation_handled = true; // Set to true ONLY on success
            } else {
                std::string status_msg = stitcher_status_to_string(status);
                // Throw an error or just report? Let's throw for now.
                throw std::runtime_error("Stitching failed: " + status_msg);
            }
        }
        else if (args.operation == "canny") {
            if (!args.canny_threshold1.has_value() || !args.canny_threshold2.has_value()) {
                // Parser provides defaults, so this check might be redundant unless defaults are removed
                throw std::runtime_error("Canny thresholds (-t1, -t2) are required for canny operation.");
            }
            std::cout << "Performing Canny edge detection..." << std::endl;
            // Using default aperture size (3) and L1 gradient
            output_image = detect_edges_canny(input_image, 
                                              args.canny_threshold1.value(), 
                                              args.canny_threshold2.value());
            operation_handled = true;
        }
        else if (args.operation == "video-gray") {
            std::cout << "Processing video to grayscale..." << std::endl;
            bool success = process_video_grayscale(args.input_files[0], args.output_file);
            if (success) {
                 std::cout << "Video processing completed successfully." << std::endl;
                 // Note: operation_handled remains false here because saving is done *inside* process_video_grayscale
                 // The generic saving block later will be skipped.
            } else {
                 // process_video_grayscale throws on error, so this might not be reached unless it returns false
                 throw std::runtime_error("Video processing failed for an unknown reason.");
            }
        }
        else if (args.operation == "detect-faces") {
            if (!args.cascade_file.has_value() || args.cascade_file.value().empty()) {
                throw std::runtime_error("Cascade file path (-c or --cascade) is required for face detection.");
            }
            std::cout << "Performing face detection..." << std::endl;
            // Using default scale factor, min neighbors, min size from detect_faces signature
            output_image = detect_faces(input_image, args.cascade_file.value()); 
            operation_handled = true; // We want to save the output image with rectangles
        }
        else if (args.operation == "bg-subtract") {
            std::cout << "Performing background subtraction..." << std::endl;
            // Using default MOG2 parameters from function signature
            bool success = process_video_bg_subtract_mog2(args.input_files[0], args.output_file);
            if (success) {
                 std::cout << "Background subtraction completed successfully." << std::endl;
                 // Saving is handled internally, operation_handled remains false.
            } else {
                 throw std::runtime_error("Background subtraction failed for an unknown reason.");
            }
        }
        // --- Add other operations here later ---
        else {
            // Unknown operation (stitch and video cases handled within their 'else if' blocks)
            throw std::runtime_error("Unknown or unimplemented operation: " + args.operation);
        }

        // --- Image Saving ---
        // This block now only runs for operations that produce a single output_image Mat
        // Video processing handles its own saving internally.
        if (operation_handled && !output_image.empty()) {
            if (!cv::imwrite(args.output_file, output_image)) {
                throw std::runtime_error("Failed to save output image to: " + args.output_file);
            }
            std::cout << "Output image saved successfully to: " << args.output_file << std::endl;
        }

        // Optional: Display images for debugging
        // if (!input_image.empty()) cv::imshow("Input Image", input_image);
        // if (!output_image.empty()) cv::imshow("Output Image", output_image);
        // if (!input_image.empty() || !output_image.empty()) cv::waitKey(0); // Wait for a key press

    } catch (const std::exception& e) { // Catch standard exceptions (incl. runtime_error, invalid_argument)
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; // Indicate error
    } catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
        return 1; // Indicate error
    }

    return 0; // Indicate successful execution (or planning thereof)
} 