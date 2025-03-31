#include "canny.hpp"
#include <stdexcept> // For std::invalid_argument

cv::Mat detect_edges_canny(const cv::Mat& input_image,
                           double threshold1,
                           double threshold2,
                           int aperture_size,
                           bool l2_gradient) 
{
    if (input_image.empty()) {
        throw std::invalid_argument("Input image for Canny edge detection is empty.");
    }
    if (threshold1 < 0 || threshold2 < 0) {
        throw std::invalid_argument("Canny thresholds must be non-negative.");
    }
    // Optional: Check aperture_size is odd and >= 3? cv::Canny might handle this.

    cv::Mat gray_image;
    // Convert to grayscale if the input image is not already grayscale
    if (input_image.channels() > 1) {
        cv::cvtColor(input_image, gray_image, cv::COLOR_BGR2GRAY); 
        // Assumes BGR input if multi-channel. Adjust if other formats are expected.
    } else {
        gray_image = input_image; // Already grayscale
    }

    // It's often recommended to blur the image slightly before Canny to reduce noise
    // For example: cv::GaussianBlur(gray_image, gray_image, cv::Size(3,3), 0);
    // Skipping blur for now to match basic requirements, but consider adding it.

    cv::Mat edges;
    // Apply the Canny edge detector
    cv::Canny(gray_image,      // Input grayscale image
              edges,           // Output edge map
              threshold1,      // Lower threshold for hysteresis
              threshold2,      // Upper threshold for hysteresis
              aperture_size,   // Aperture size for Sobel operator
              l2_gradient);    // L2 gradient flag

    return edges;
} 