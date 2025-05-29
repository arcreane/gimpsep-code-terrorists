#include <opencv2/opencv.hpp>
#include "advanced/face_detection.hpp"
#include <iostream>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <input_image> <output_image>" << std::endl;
        return 1;
    }

    try {
        // Load the input image
        cv::Mat input = cv::imread(argv[1]);
        if (input.empty()) {
            throw std::runtime_error("Failed to load input image: " + std::string(argv[1]));
        }

        // Create face detector
        // Using the local Haar cascade file
        std::string cascade_path = "../models/haarcascades/haarcascade_frontalface_default.xml";
        ai_slop::FaceDetector detector(cascade_path, 1.05, 5, cv::Size(40, 40));

        // Process the image
        cv::Mat result = detector.process(input);

        // Save the result
        if (!cv::imwrite(argv[2], result)) {
            throw std::runtime_error("Failed to save output image: " + std::string(argv[2]));
        }

        std::cout << "Face detection completed successfully!" << std::endl;
        std::cout << "Output saved to: " << argv[2] << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
} 