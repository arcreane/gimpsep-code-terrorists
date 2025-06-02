#include "brightness.hpp"
#include <stdexcept> // For std::invalid_argument

cv::Mat adjust_brightness(const cv::Mat& input_image, int value) {
    if (input_image.empty()) {
        throw std::invalid_argument("Input image for brightness adjustment is empty.");
    }

    // Create an output image of the same size and type as the input
    cv::Mat adjusted_image;

    // Add the scalar value to the input image.
    // OpenCV's add function handles saturation automatically for standard types like CV_8U.
    // It ensures pixel values stay within the valid range [0, 255].
    cv::add(input_image, cv::Scalar(value, value, value, value), adjusted_image);
    // Note: The Scalar constructor takes up to 4 values (B, G, R, Alpha).
    // If the input image has fewer channels, the extra scalar values are ignored.
    // Using the same value for all channels provides uniform brightness adjustment.

    return adjusted_image;
} 