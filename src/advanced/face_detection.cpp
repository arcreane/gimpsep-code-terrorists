#include "face_detection.hpp"
#include <stdexcept>
#include <iostream>

namespace ai_slop {

FaceDetector::FaceDetector(const std::string& cascade_file_path,
                         double scale_factor,
                         int min_neighbors,
                         cv::Size min_size)
    : scale_factor_(scale_factor)
    , min_neighbors_(min_neighbors)
    , min_size_(min_size) {
    if (!classifier_.load(cascade_file_path)) {
        throw std::runtime_error("Failed to load cascade classifier from: " + cascade_file_path);
    }
}

cv::Mat FaceDetector::process(const cv::Mat& input) {
    validateInput(input);

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);

    // Detect faces
    std::vector<cv::Rect> faces;
    classifier_.detectMultiScale(gray, faces, scale_factor_, min_neighbors_, 0, min_size_);

    // Draw rectangles around faces
    cv::Mat result = input.clone();
    for (const auto& face : faces) {
        cv::rectangle(result, face, cv::Scalar(0, 255, 0), 2);
    }

    return result;
}

} // namespace ai_slop 