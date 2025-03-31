#ifndef AI_SLOP_RESIZE_HPP
#define AI_SLOP_RESIZE_HPP

#include <opencv2/core.hpp> // For cv::Mat
#include <opencv2/imgproc.hpp> // For cv::resize

/**
 * @brief Resizes an input image by a given factor.
 *
 * @param input_image The source image (cv::Mat).
 * @param factor The scaling factor (e.g., > 1 to enlarge, < 1 to shrink). Must be positive.
 * @param interpolation The interpolation method to use (default: cv::INTER_LINEAR).
 *                      See OpenCV documentation for options (e.g., cv::INTER_NEAREST, cv::INTER_CUBIC).
 * @return cv::Mat The resized image.
 * @throws std::invalid_argument if factor is not positive.
 */
cv::Mat resize_image(const cv::Mat& input_image,
                     double factor,
                     int interpolation = cv::INTER_LINEAR);

// Potential future overload:
// cv::Mat resize_image(const cv::Mat& input_image, int target_width, int target_height, int interpolation = cv::INTER_LINEAR);

#endif // AI_SLOP_RESIZE_HPP 