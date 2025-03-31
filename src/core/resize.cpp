#include "resize.hpp"
#include <stdexcept> // For std::invalid_argument

cv::Mat resize_image(const cv::Mat& input_image,
                     double factor,
                     int interpolation) 
{
    if (factor <= 0) {
        throw std::invalid_argument("Resize factor must be positive, received: " + std::to_string(factor));
    }

    if (input_image.empty()) {
        // Or return an empty Mat, depending on desired error handling
        throw std::invalid_argument("Input image for resize is empty."); 
    }

    cv::Mat resized_image;
    // cv::resize uses fx and fy for scaling factors.
    // Setting dsize to (0, 0) tells it to use the factors.
    cv::resize(input_image, 
               resized_image, 
               cv::Size(), // Target size (0,0 means calculate from factors)
               factor,      // Scale factor along X axis
               factor,      // Scale factor along Y axis
               interpolation);

    return resized_image;
}

// Implementation for potential future overload:
/*
cv::Mat resize_image(const cv::Mat& input_image, int target_width, int target_height, int interpolation) {
    if (target_width <= 0 || target_height <= 0) {
        throw std::invalid_argument("Target width and height must be positive.");
    }
    if (input_image.empty()) {
        throw std::invalid_argument("Input image for resize is empty."); 
    }

    cv::Mat resized_image;
    cv::resize(input_image, 
               resized_image, 
               cv::Size(target_width, target_height),
               0, // fx (unused when dsize is specified)
               0, // fy (unused when dsize is specified)
               interpolation);

    return resized_image;
}
*/ 