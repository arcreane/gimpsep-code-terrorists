#include "morphology.hpp"
#include <stdexcept> // For std::invalid_argument

/**
 * @brief Validates the kernel size for morphological operations.
 *
 * @param kernel_size The size to validate.
 * @throws std::invalid_argument if kernel_size is not a positive odd integer.
 */
static void validate_kernel_size(int kernel_size) {
    if (kernel_size <= 0 || kernel_size % 2 == 0) {
        throw std::invalid_argument("Kernel size must be a positive odd integer, received: " + std::to_string(kernel_size));
    }
}

cv::Mat dilate_image(const cv::Mat& input_image, int kernel_size) {
    validate_kernel_size(kernel_size);

    // Create the structuring element (kernel) for dilation
    // Using a rectangular element here, could also use MORPH_CROSS or MORPH_ELLIPSE
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                            cv::Size(kernel_size, kernel_size));

    cv::Mat dilated_image;
    // Perform dilation
    cv::dilate(input_image, dilated_image, element);

    return dilated_image;
}

cv::Mat erode_image(const cv::Mat& input_image, int kernel_size) {
    validate_kernel_size(kernel_size);

    // Create the structuring element (kernel) for erosion
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                            cv::Size(kernel_size, kernel_size));

    cv::Mat eroded_image;
    // Perform erosion
    cv::erode(input_image, eroded_image, element);

    return eroded_image;
} 