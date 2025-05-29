#include "cli_interface.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "core/morphology_processor.hpp"
#include "core/brightness_processor.hpp"
#include "core/resize_processor.hpp"
#include "core/canny_processor.hpp"
#include "core/stitch_processor.hpp"

namespace ai_slop {

void CLIInterface::run() {
    while (true) {
        std::cout << "\n=== AI_SLOP Image Processing Tool ===\n\n";
        std::cout << "Available Operations:\n";
        std::cout << "1. Dilation\n";
        std::cout << "2. Erosion\n";
        std::cout << "3. Resize\n";
        std::cout << "4. Brightness Adjustment\n";
        std::cout << "5. Canny Edge Detection\n";
        std::cout << "6. Image Stitching\n";
        std::cout << "7. Exit\n\n";
        
        std::cout << "Select an operation (1-7): ";
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline

        if (choice == 7) {
            std::cout << "Goodbye!\n";
            break;
        }

        try {
            switch (choice) {
                case 1: handleDilation(); break;
                case 2: handleErosion(); break;
                case 3: handleResize(); break;
                case 4: handleBrightness(); break;
                case 5: handleCanny(); break;
                case 6: handleStitch(); break;
                default:
                    std::cout << "Invalid choice. Please try again.\n";
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}

void CLIInterface::handleDilation() {
    std::cout << "\n=== Dilation Operation ===\n";
    std::string input_path = getInputImagePath();
    int kernel_size = getKernelSize();
    std::string output_path = getOutputPath("dilate");

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>("AI_SLOP"));
    argv.push_back(const_cast<char*>("--operation"));
    argv.push_back(const_cast<char*>("dilate"));
    argv.push_back(const_cast<char*>("-i"));
    argv.push_back(const_cast<char*>(input_path.c_str()));
    argv.push_back(const_cast<char*>("-o"));
    argv.push_back(const_cast<char*>(output_path.c_str()));
    argv.push_back(const_cast<char*>("--kernel_size"));
    argv.push_back(const_cast<char*>(std::to_string(kernel_size).c_str()));

    ParsedArguments args = parse_arguments(argv.size(), argv.data());
    processImage(args);
}

void CLIInterface::handleErosion() {
    std::cout << "\n=== Erosion Operation ===\n";
    std::string input_path = getInputImagePath();
    int kernel_size = getKernelSize();
    std::string output_path = getOutputPath("erode");

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>("AI_SLOP"));
    argv.push_back(const_cast<char*>("--operation"));
    argv.push_back(const_cast<char*>("erode"));
    argv.push_back(const_cast<char*>("-i"));
    argv.push_back(const_cast<char*>(input_path.c_str()));
    argv.push_back(const_cast<char*>("-o"));
    argv.push_back(const_cast<char*>(output_path.c_str()));
    argv.push_back(const_cast<char*>("--kernel_size"));
    argv.push_back(const_cast<char*>(std::to_string(kernel_size).c_str()));

    ParsedArguments args = parse_arguments(argv.size(), argv.data());
    processImage(args);
}

void CLIInterface::handleResize() {
    std::cout << "\n=== Resize Operation ===\n";
    std::string input_path = getInputImagePath();
    double factor = getResizeFactor();
    std::string output_path = getOutputPath("resized");

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>("AI_SLOP"));
    argv.push_back(const_cast<char*>("--operation"));
    argv.push_back(const_cast<char*>("resize"));
    argv.push_back(const_cast<char*>("-i"));
    argv.push_back(const_cast<char*>(input_path.c_str()));
    argv.push_back(const_cast<char*>("-o"));
    argv.push_back(const_cast<char*>(output_path.c_str()));
    argv.push_back(const_cast<char*>("--factor"));
    argv.push_back(const_cast<char*>(std::to_string(factor).c_str()));

    ParsedArguments args = parse_arguments(argv.size(), argv.data());
    processImage(args);
}

void CLIInterface::handleBrightness() {
    std::cout << "\n=== Brightness Adjustment ===\n";
    std::string input_path = getInputImagePath();
    int brightness = getBrightnessValue();
    std::string output_path = getOutputPath("brightness_adjusted");

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>("AI_SLOP"));
    argv.push_back(const_cast<char*>("--operation"));
    argv.push_back(const_cast<char*>("brightness"));
    argv.push_back(const_cast<char*>("-i"));
    argv.push_back(const_cast<char*>(input_path.c_str()));
    argv.push_back(const_cast<char*>("-o"));
    argv.push_back(const_cast<char*>(output_path.c_str()));
    argv.push_back(const_cast<char*>("--brightness"));
    argv.push_back(const_cast<char*>(std::to_string(brightness).c_str()));

    ParsedArguments args = parse_arguments(argv.size(), argv.data());
    processImage(args);
}

void CLIInterface::handleCanny() {
    std::cout << "\n=== Canny Edge Detection ===\n";
    std::string input_path = getInputImagePath();
    double threshold1 = getCannyThreshold1();
    double threshold2 = getCannyThreshold2();
    std::string output_path = getOutputPath("edges");

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>("AI_SLOP"));
    argv.push_back(const_cast<char*>("--operation"));
    argv.push_back(const_cast<char*>("canny"));
    argv.push_back(const_cast<char*>("-i"));
    argv.push_back(const_cast<char*>(input_path.c_str()));
    argv.push_back(const_cast<char*>("-o"));
    argv.push_back(const_cast<char*>(output_path.c_str()));
    argv.push_back(const_cast<char*>("--threshold1"));
    argv.push_back(const_cast<char*>(std::to_string(threshold1).c_str()));
    argv.push_back(const_cast<char*>("--threshold2"));
    argv.push_back(const_cast<char*>(std::to_string(threshold2).c_str()));

    ParsedArguments args = parse_arguments(argv.size(), argv.data());
    processImage(args);
}

void CLIInterface::handleStitch() {
    std::cout << "\n=== Image Stitching ===\n";
    std::vector<std::string> input_paths = getMultipleInputPaths();
    std::string output_path = getOutputPath("stitched");

    std::vector<char*> argv;
    argv.push_back(const_cast<char*>("AI_SLOP"));
    argv.push_back(const_cast<char*>("--operation"));
    argv.push_back(const_cast<char*>("stitch"));
    argv.push_back(const_cast<char*>("-o"));
    argv.push_back(const_cast<char*>(output_path.c_str()));
    for (const auto& path : input_paths) {
        argv.push_back(const_cast<char*>("-i"));
        argv.push_back(const_cast<char*>(path.c_str()));
    }

    ParsedArguments args = parse_arguments(argv.size(), argv.data());
    processImage(args);
}

std::string CLIInterface::prependInputFolder(const std::string& filename) {
    // If the path starts with ./ or ../, return it as is
    if (filename.substr(0, 2) == "./" || filename.substr(0, 3) == "../") {
        return filename;
    }
    
    // If the path starts with /, return it as is (absolute path)
    if (filename[0] == '/') {
        return filename;
    }
    
    // Otherwise, try to find the image in the current directory
    return filename;
}

std::string CLIInterface::prependOutputFolder(const std::string& filename) {
    // If the path starts with ./ or ../, return it as is
    if (filename.substr(0, 2) == "./" || filename.substr(0, 3) == "../") {
        return filename;
    }
    
    // If the path starts with /, return it as is (absolute path)
    if (filename[0] == '/') {
        return filename;
    }
    
    // Otherwise, save in the current directory
    return filename;
}

std::string CLIInterface::getInputImagePath() {
    std::string path;
    std::cout << "Enter input image filename or path: ";
    std::getline(std::cin, path);
    return prependInputFolder(path);
}

std::vector<std::string> CLIInterface::getMultipleInputPaths() {
    std::vector<std::string> paths;
    std::cout << "Enter input image filenames or paths (one per line, empty line to finish):\n";
    std::string path;
    while (true) {
        std::getline(std::cin, path);
        if (path.empty()) break;
        paths.push_back(prependInputFolder(path));
    }
    return paths;
}

std::string CLIInterface::getOutputPath(const std::string& operation) {
    std::string path;
    std::cout << "Enter output filename or path (or press Enter for default): ";
    std::getline(std::cin, path);
    if (path.empty()) {
        path = operation + "_output.jpg";
    }
    return prependOutputFolder(path);
}

int CLIInterface::getKernelSize() {
    int size;
    std::cout << "Enter kernel size (positive odd number): ";
    std::cin >> size;
    std::cin.ignore();
    return size;
}

double CLIInterface::getResizeFactor() {
    double factor;
    std::cout << "Enter resize factor (e.g., 0.5 for half size, 2.0 for double size): ";
    std::cin >> factor;
    std::cin.ignore();
    return factor;
}

int CLIInterface::getBrightnessValue() {
    int value;
    std::cout << "Enter brightness adjustment (-255 to 255): ";
    std::cin >> value;
    std::cin.ignore();
    return value;
}

double CLIInterface::getCannyThreshold1() {
    double value;
    std::cout << "Enter first threshold (default: 100): ";
    std::cin >> value;
    std::cin.ignore();
    return value;
}

double CLIInterface::getCannyThreshold2() {
    double value;
    std::cout << "Enter second threshold (default: 200): ";
    std::cin >> value;
    std::cin.ignore();
    return value;
}

void CLIInterface::processImage(const ParsedArguments& args) {
    // Debug: print input file path
    std::cout << "[DEBUG] Loading input image from: " << args.input_files[0] << std::endl;
    cv::Mat input_image = cv::imread(args.input_files[0]);
    if (input_image.empty()) {
        throw std::runtime_error("Failed to load input image: " + args.input_files[0]);
    }
    // Debug: print input image size
    std::cout << "[DEBUG] Input image size: " << input_image.size() << std::endl;

    // Process the image based on operation
    cv::Mat output_image;
    if (args.operation == "dilate") {
        std::cout << "[DEBUG] Using MorphologyProcessor(Dilate) with kernel size: " << args.kernel_size.value() << std::endl;
        MorphologyProcessor processor(MorphologyProcessor::Dilate, args.kernel_size.value());
        output_image = processor.process(input_image);
    }
    else if (args.operation == "erode") {
        std::cout << "[DEBUG] Using MorphologyProcessor(Erode) with kernel size: " << args.kernel_size.value() << std::endl;
        MorphologyProcessor processor(MorphologyProcessor::Erode, args.kernel_size.value());
        output_image = processor.process(input_image);
    }
    else if (args.operation == "resize") {
        std::cout << "[DEBUG] Using ResizeProcessor with factor: " << args.resize_factor.value() << std::endl;
        ResizeProcessor processor(args.resize_factor.value());
        output_image = processor.process(input_image);
    }
    else if (args.operation == "brightness") {
        std::cout << "[DEBUG] Using BrightnessProcessor with value: " << args.brightness_value.value() << std::endl;
        BrightnessProcessor processor(args.brightness_value.value());
        output_image = processor.process(input_image);
    }
    else if (args.operation == "canny") {
        std::cout << "[DEBUG] Using CannyProcessor with thresholds: " << args.canny_threshold1.value() << ", " << args.canny_threshold2.value() << std::endl;
        CannyProcessor processor(args.canny_threshold1.value(), args.canny_threshold2.value());
        output_image = processor.process(input_image);
    }
    else if (args.operation == "stitch") {
        std::cout << "[DEBUG] Using StitchProcessor with " << args.input_files.size() << " input files" << std::endl;
        StitchProcessor processor(args.input_files);
        output_image = processor.process(input_image);
    }

    // Debug: print output image size
    std::cout << "[DEBUG] Output image size: " << output_image.size() << std::endl;

    // Debug: print output file path
    std::cout << "[DEBUG] Saving output image to: " << args.output_file << std::endl;
    if (!cv::imwrite(args.output_file, output_image)) {
        throw std::runtime_error("Failed to save output image: " + args.output_file);
    }

    std::cout << "Processing complete! Output saved to: " << args.output_file << "\n";

    // Show preview
    std::cout << "Would you like to see a preview? (y/n): ";
    char response;
    std::cin >> response;
    if (response == 'y' || response == 'Y') {
        cv::imshow("Input", input_image);
        cv::imshow("Output", output_image);
        cv::waitKey(0);
        cv::destroyAllWindows();
    }
}

} // namespace ai_slop 