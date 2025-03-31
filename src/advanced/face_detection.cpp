#include "face_detection.hpp"
#include <stdexcept>
#include <iostream>

cv::Mat detect_faces(const cv::Mat& input_image,
                     const std::string& cascade_file_path,
                     double scale_factor,
                     int min_neighbors,
                     cv::Size min_size)
{
    if (input_image.empty()) {
        throw std::runtime_error("Input image for face detection is empty.");
    }

    // 1. Load the cascade classifier
    cv::CascadeClassifier face_cascade;
    if (!face_cascade.load(cascade_file_path)) {
        throw std::runtime_error("Error loading face cascade file: " + cascade_file_path + ". Make sure the file exists and is accessible.");
    }
    std::cout << "Loaded face cascade: " << cascade_file_path << std::endl;

    // 2. Convert to grayscale
    cv::Mat gray_image;
    if (input_image.channels() == 3) {
         cv::cvtColor(input_image, gray_image, cv::COLOR_BGR2GRAY);
    } else if (input_image.channels() == 1) {
         gray_image = input_image.clone(); // Already grayscale
    } else {
        throw std::runtime_error("Unsupported number of channels in input image for face detection.");
    }

    // 3. Apply histogram equalization (improves contrast, often helps detection)
    cv::equalizeHist(gray_image, gray_image);

    // 4. Detect faces
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray_image,          // Input grayscale image
                                  faces,             // Vector of rectangles where faces are found
                                  scale_factor,      // Scale factor
                                  min_neighbors,     // Min neighbors
                                  0 | cv::CASCADE_SCALE_IMAGE, // Flags (default is fine)
                                  min_size);         // Minimum face size

    std::cout << "Detected " << faces.size() << " faces." << std::endl;

    // 5. Draw rectangles on a *copy* of the original color image
    cv::Mat output_image = input_image.clone();
    for (const auto& rect : faces) {
        // Draw a green rectangle around each detected face
        cv::rectangle(output_image,
                      rect,
                      cv::Scalar(0, 255, 0), // Color (BGR Green)
                      2);                   // Thickness
    }

    return output_image;
} 