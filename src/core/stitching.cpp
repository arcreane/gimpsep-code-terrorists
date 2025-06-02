#include "stitching.hpp"
#include <opencv2/imgcodecs.hpp> // For cv::imread
#include <stdexcept>            // For std::runtime_error
#include <iostream>             // For status messages

cv::Stitcher::Status stitch_images(const std::vector<std::string>& image_paths, cv::Mat& output_pano) {
    if (image_paths.size() < 2) {
        throw std::runtime_error("Stitching requires at least two input images.");
    }

    std::vector<cv::Mat> input_images;
    input_images.reserve(image_paths.size()); // Reserve space for efficiency

    std::cout << "Loading images for stitching..." << std::endl;
    for (const std::string& path : image_paths) {
        cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
        if (img.empty()) {
            // You might want to skip the problematic image or throw an error
            // Throwing an error is safer to indicate a definite problem.
            throw std::runtime_error("Failed to load image for stitching: " + path);
        }
        input_images.push_back(img);
        std::cout << "  Loaded: " << path << std::endl;
    }

    std::cout << "Attempting to stitch images..." << std::endl;

    // Create a Stitcher instance
    // Use default mode (PANORAMA) or specify SCANS for potentially different behavior
    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    // Potential alternative: cv::Stitcher::create(cv::Stitcher::SCANS);

    // Attempt to stitch the images
    cv::Stitcher::Status status = stitcher->stitch(input_images, output_pano);

    return status; // Return the status code
}

std::string stitcher_status_to_string(cv::Stitcher::Status status) {
    switch (status) {
        case cv::Stitcher::OK:
            return "OK (Success)";
        case cv::Stitcher::ERR_NEED_MORE_IMGS:
            return "Error: Need more images";
        case cv::Stitcher::ERR_HOMOGRAPHY_EST_FAIL:
            return "Error: Homography estimation failed";
        case cv::Stitcher::ERR_CAMERA_PARAMS_ADJUST_FAIL:
            return "Error: Camera parameter adjustment failed";
        default:
            return "Error: Unknown stitching error";
    }
} 