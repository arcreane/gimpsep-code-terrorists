#ifndef AI_SLOP_MORPHOLOGY_HPP
#define AI_SLOP_MORPHOLOGY_HPP

#include <opencv2/core.hpp> // For cv::Mat
#include <opencv2/imgproc.hpp> // For morphological operations

/**
 * @brief Dilates an input image.
 *
 * @param input_image The source image (cv::Mat).
 * @param kernel_size The size of the structuring element kernel (must be a positive odd integer).
 * @return cv::Mat The dilated image.
 * @throws std::invalid_argument if kernel_size is not a positive odd integer.
 */
cv::Mat dilate_image(const cv::Mat& input_image, int kernel_size);

/**
 * @brief Erodes an input image.
 *
 * @param input_image The source image (cv::Mat).
 * @param kernel_size The size of the structuring element kernel (must be a positive odd integer).
 * @return cv::Mat The eroded image.
 * @throws std::invalid_argument if kernel_size is not a positive odd integer.
 */
cv::Mat erode_image(const cv::Mat& input_image, int kernel_size);

#endif // AI_SLOP_MORPHOLOGY_HPP 