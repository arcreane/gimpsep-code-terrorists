#include "menu_interface.hpp"
#include <iostream>
#include <limits>
#include <filesystem>
#include <iomanip>
#include <opencv2/opencv.hpp>

// Include processor headers
#include "core/brightness_processor.hpp"
#include "core/morphology_processor.hpp"
#include "core/resize_processor.hpp"
#include "core/canny_processor.hpp"
#include "core/stitch_processor.hpp"
#include "advanced/face_detection.hpp"
#include "advanced/object_detection.hpp"
#include "advanced/video_processing.hpp"

namespace ai_slop {

MenuInterface::MenuInterface() : running(true) {
    // Initialize main menu actions
    main_menu_actions = {
        {"1", [this]() { executeOperation("dilate", collectMorphologyParams()); }},
        {"2", [this]() { executeOperation("erode", collectMorphologyParams()); }},
        {"3", [this]() { executeOperation("resize", collectResizeParams()); }},
        {"4", [this]() { executeOperation("brightness", collectBrightnessParams()); }},
        {"5", [this]() { executeOperation("canny", collectCannyParams()); }},
        {"6", [this]() { executeOperation("stitch", collectStitchParams()); }},
        {"7", [this]() { executeOperation("face", collectFaceDetectionParams()); }},
        {"8", [this]() { executeOperation("object", collectObjectDetectionParams()); }},
        {"9", [this]() { executeOperation("video_grayscale", collectVideoGrayscaleParams()); }},
        {"h", [this]() { showHelp(); }},
        {"q", [this]() { running = false; }}
    };

    // Initialize operation descriptions
    operation_descriptions = {
        {"dilate", "Dilate an image using morphological operations"},
        {"erode", "Erode an image using morphological operations"},
        {"resize", "Resize an image by a specified factor"},
        {"brightness", "Adjust the brightness of an image"},
        {"canny", "Perform Canny edge detection on an image"},
        {"stitch", "Stitch multiple images together"},
        {"face", "Detect faces in an image"},
        {"object", "Detect objects in an image using YOLO"},
        {"video_grayscale", "Convert a video to grayscale"}
    };
}

void MenuInterface::run() {
    while (running) {
        showMainMenu();
        std::string choice = getStringInput("Enter your choice: ");
        
        auto it = main_menu_actions.find(choice);
        if (it != main_menu_actions.end()) {
            try {
                it->second();
            } catch (const std::exception& e) {
                printError(e.what());
            }
        } else {
            printError("Invalid choice. Please try again.");
        }
    }
}

void MenuInterface::showMainMenu() {
    std::cout << "\n=== AI_SLOP Image Processing Menu ===\n\n";
    std::cout << "1. Dilate Image\n";
    std::cout << "2. Erode Image\n";
    std::cout << "3. Resize Image\n";
    std::cout << "4. Adjust Brightness\n";
    std::cout << "5. Canny Edge Detection\n";
    std::cout << "6. Stitch Images\n";
    std::cout << "7. Face Detection\n";
    std::cout << "8. Object Detection\n";
    std::cout << "9. Video Grayscale\n";
    std::cout << "h. Show Help\n";
    std::cout << "q. Quit\n\n";
}

void MenuInterface::showHelp(const std::string& operation) {
    if (operation.empty()) {
        std::cout << "\n=== AI_SLOP Help ===\n\n";
        std::cout << "This application provides various image processing operations:\n\n";
        for (const auto& [op, desc] : operation_descriptions) {
            std::cout << std::setw(12) << std::left << op << ": " << desc << "\n";
        }
        std::cout << "\nType 'h' in any menu for help, or 'q' to quit.\n";
    } else {
        auto it = operation_descriptions.find(operation);
        if (it != operation_descriptions.end()) {
            std::cout << "\n=== Help for " << operation << " ===\n\n";
            std::cout << it->second << "\n";
            // Add specific help for each operation
            if (operation == "dilate" || operation == "erode") {
                std::cout << "Kernel size determines the size of the morphological operation.\n";
            } else if (operation == "resize") {
                std::cout << "Resize factor should be greater than 0.\n";
            } else if (operation == "brightness") {
                std::cout << "Brightness value should be between -255 and 255.\n";
            } else if (operation == "canny") {
                std::cout << "Threshold values determine edge detection sensitivity.\n";
            } else if (operation == "stitch") {
                std::cout << "Select multiple images to stitch together.\n";
            } else if (operation == "face") {
                std::cout << "Detects faces in the input image using Haar cascades.\n";
                std::cout << "Scale factor determines how much the image size is reduced at each scale.\n";
                std::cout << "Min neighbors determines how many neighbors each candidate rectangle should have.\n";
            } else if (operation == "object") {
                std::cout << "Detects objects in the input image using YOLO.\n";
                std::cout << "Confidence threshold determines the minimum confidence score for detection.\n";
                std::cout << "NMS threshold determines the threshold for non-maximum suppression.\n";
            } else if (operation == "video_grayscale") {
                std::cout << "Converts a video to grayscale.\n";
            }
        }
    }
}

std::string MenuInterface::getStringInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::getline(std::cin, input);
    return input;
}

int MenuInterface::getIntInput(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        try {
            value = std::stoi(input);
            if (value >= min && value <= max) {
                return value;
            }
            std::cout << "Please enter a value between " << min << " and " << max << ".\n";
        } catch (const std::exception&) {
            std::cout << "Please enter a valid number.\n";
        }
    }
}

double MenuInterface::getDoubleInput(const std::string& prompt, double min, double max) {
    double value;
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        try {
            value = std::stod(input);
            if (value >= min && value <= max) {
                return value;
            }
            std::cout << "Please enter a value between " << min << " and " << max << ".\n";
        } catch (const std::exception&) {
            std::cout << "Please enter a valid number.\n";
        }
    }
}

std::string MenuInterface::getInputPath() {
    while (true) {
        std::string path = getStringInput("Enter input image path: ");
        if (validateInputPath(path)) {
            return path;
        }
        printError("Invalid input path. Please try again.");
    }
}

std::string MenuInterface::getOutputPath() {
    while (true) {
        std::string path = getStringInput("Enter output image path: ");
        if (validateOutputPath(path)) {
            return path;
        }
        printError("Invalid output path. Please try again.");
    }
}

OperationParams MenuInterface::collectBasicParams() {
    OperationParams params;
    params.input_path = getInputPath();
    params.output_path = getOutputPath();
    return params;
}

OperationParams MenuInterface::collectMorphologyParams() {
    OperationParams params = collectBasicParams();
    params.options["kernel_size"] = std::to_string(
        getIntInput("Enter kernel size (1-31): ", 1, 31)
    );
    return params;
}

OperationParams MenuInterface::collectResizeParams() {
    OperationParams params = collectBasicParams();
    params.options["factor"] = std::to_string(
        getDoubleInput("Enter resize factor (0.1-10.0): ", 0.1, 10.0)
    );
    return params;
}

OperationParams MenuInterface::collectBrightnessParams() {
    OperationParams params = collectBasicParams();
    params.options["value"] = std::to_string(
        getIntInput("Enter brightness value (-255 to 255): ", -255, 255)
    );
    return params;
}

OperationParams MenuInterface::collectCannyParams() {
    OperationParams params = collectBasicParams();
    params.options["threshold1"] = std::to_string(
        getDoubleInput("Enter first threshold (0-255): ", 0, 255)
    );
    params.options["threshold2"] = std::to_string(
        getDoubleInput("Enter second threshold (0-255): ", 0, 255)
    );
    return params;
}

OperationParams MenuInterface::collectStitchParams() {
    OperationParams params = collectBasicParams();
    int num_images = getIntInput("Enter number of images to stitch (2-10): ", 2, 10);
    for (int i = 0; i < num_images; ++i) {
        std::string path = getStringInput("Enter path for image " + std::to_string(i + 1) + ": ");
        if (validateInputPath(path)) {
            params.options["input" + std::to_string(i)] = path;
        } else {
            printError("Invalid input path. Please try again.");
            --i;
        }
    }
    return params;
}

OperationParams MenuInterface::collectFaceDetectionParams() {
    OperationParams params = collectBasicParams();
    
    // Get scale factor (1.1 to 2.0)
    params.options["scale_factor"] = std::to_string(
        getDoubleInput("Enter scale factor (1.1-2.0): ", 1.1, 2.0)
    );
    
    // Get min neighbors (1 to 10)
    params.options["min_neighbors"] = std::to_string(
        getIntInput("Enter minimum neighbors (1-10): ", 1, 10)
    );
    
    // Get min size (20 to 100)
    params.options["min_size"] = std::to_string(
        getIntInput("Enter minimum face size (20-100): ", 20, 100)
    );
    
    return params;
}

OperationParams MenuInterface::collectObjectDetectionParams() {
    OperationParams params = collectBasicParams();
    
    // Get confidence threshold (0.1 to 1.0)
    params.options["confidence"] = std::to_string(
        getDoubleInput("Enter confidence threshold (0.1-1.0): ", 0.1, 1.0)
    );
    
    // Get NMS threshold (0.1 to 1.0)
    params.options["nms"] = std::to_string(
        getDoubleInput("Enter NMS threshold (0.1-1.0): ", 0.1, 1.0)
    );
    
    return params;
}

OperationParams MenuInterface::collectVideoGrayscaleParams() {
    OperationParams params;
    params.input_path = getStringInput("Enter input video path: ");
    params.output_path = getStringInput("Enter output video path: ");
    return params;
}

void MenuInterface::executeOperation(const std::string& operation, const OperationParams& params) {
    printProgress("Processing...");
    
    if (operation == "video_grayscale") {
        processVideo(operation, params);
    } else {
        processImage(operation, params);
    }
    
    printSuccess("Processing completed successfully!");
}

void MenuInterface::processImage(const std::string& operation, const OperationParams& params) {
    // Load input image
    cv::Mat input_image = cv::imread(params.input_path);
    if (input_image.empty()) {
        throw std::runtime_error("Failed to load input image: " + params.input_path);
    }

    cv::Mat output_image;

    // Process based on operation type
    if (operation == "dilate") {
        auto it = params.options.find("kernel_size");
        if (it == params.options.end()) {
            throw std::runtime_error("Missing kernel_size parameter");
        }
        int kernel_size = std::stoi(it->second);
        MorphologyProcessor processor(MorphologyProcessor::Dilate, kernel_size);
        output_image = processor.process(input_image);
    }
    else if (operation == "erode") {
        auto it = params.options.find("kernel_size");
        if (it == params.options.end()) {
            throw std::runtime_error("Missing kernel_size parameter");
        }
        int kernel_size = std::stoi(it->second);
        MorphologyProcessor processor(MorphologyProcessor::Erode, kernel_size);
        output_image = processor.process(input_image);
    }
    else if (operation == "resize") {
        auto it = params.options.find("factor");
        if (it == params.options.end()) {
            throw std::runtime_error("Missing factor parameter");
        }
        double factor = std::stod(it->second);
        ResizeProcessor processor(factor);
        output_image = processor.process(input_image);
    }
    else if (operation == "brightness") {
        auto it = params.options.find("value");
        if (it == params.options.end()) {
            throw std::runtime_error("Missing value parameter");
        }
        int value = std::stoi(it->second);
        BrightnessProcessor processor(value);
        output_image = processor.process(input_image);
    }
    else if (operation == "canny") {
        auto it1 = params.options.find("threshold1");
        auto it2 = params.options.find("threshold2");
        if (it1 == params.options.end() || it2 == params.options.end()) {
            throw std::runtime_error("Missing threshold parameters");
        }
        double threshold1 = std::stod(it1->second);
        double threshold2 = std::stod(it2->second);
        CannyProcessor processor(threshold1, threshold2);
        output_image = processor.process(input_image);
    }
    else if (operation == "stitch") {
        std::vector<std::string> input_files;
        for (const auto& [key, value] : params.options) {
            if (key.length() >= 5 && key.substr(0, 5) == "input") {
                input_files.push_back(value);
            }
        }
        StitchProcessor processor(input_files);
        output_image = processor.process(input_image);
    }
    else if (operation == "face") {
        auto it1 = params.options.find("scale_factor");
        auto it2 = params.options.find("min_neighbors");
        auto it3 = params.options.find("min_size");
        if (it1 == params.options.end() || it2 == params.options.end() || it3 == params.options.end()) {
            throw std::runtime_error("Missing face detection parameters");
        }
        
        double scale_factor = std::stod(it1->second);
        int min_neighbors = std::stoi(it2->second);
        int min_size = std::stoi(it3->second);
        
        // Use the Haar cascade file from the models directory
        std::string cascade_path = "../models/haarcascades/haarcascade_frontalface_default.xml";
        FaceDetector detector(cascade_path, scale_factor, min_neighbors, cv::Size(min_size, min_size));
        
        output_image = detector.process(input_image);
    }
    else if (operation == "object") {
        auto it1 = params.options.find("confidence");
        auto it2 = params.options.find("nms");
        if (it1 == params.options.end() || it2 == params.options.end()) {
            throw std::runtime_error("Missing object detection parameters");
        }
        
        float confidence = std::stof(it1->second);
        float nms = std::stof(it2->second);
        
        // Use the YOLO model files from the models directory
        std::string config_path = "../models/yolo/yolov4-tiny.cfg";
        std::string weights_path = "../models/yolo/yolov4-tiny.weights";
        std::string names_path = "../models/yolo/coco.names";
        
        ObjectDetector detector(config_path, weights_path, names_path, confidence, nms);
        output_image = detector.process(input_image);
    }
    else {
        throw std::runtime_error("Unknown operation: " + operation);
    }

    // Save the output image
    if (!cv::imwrite(params.output_path, output_image)) {
        throw std::runtime_error("Failed to save output image: " + params.output_path);
    }
}

void MenuInterface::processVideo(const std::string& operation, const OperationParams& params) {
    if (operation == "video_grayscale") {
        process_video_grayscale(params.input_path, params.output_path);
    } else {
        throw std::runtime_error("Unknown video operation: " + operation);
    }
}

void MenuInterface::printError(const std::string& message) {
    std::cerr << "Error: " << message << std::endl;
}

void MenuInterface::printSuccess(const std::string& message) {
    std::cout << "Success: " << message << std::endl;
}

void MenuInterface::printProgress(const std::string& message) {
    std::cout << message << std::endl;
}

bool MenuInterface::validateInputPath(const std::string& path) {
    return std::filesystem::exists(path);
}

bool MenuInterface::validateOutputPath(const std::string& path) {
    std::filesystem::path output_dir = std::filesystem::path(path).parent_path();
    return std::filesystem::exists(output_dir);
}

} // namespace ai_slop 