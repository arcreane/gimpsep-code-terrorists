#ifndef AI_SLOP_BRIGHTNESS_HPP
#define AI_SLOP_BRIGHTNESS_HPP

#include <opencv2/core.hpp> // For cv::Mat

/**
 * @brief Adjusts the brightness of an input image.
 *
 * Adds a specified value to each pixel intensity. Values are clamped
 * to the valid range [0, 255] for standard 8-bit images.
 *
 * @param input_image The source image (cv::Mat, expected to be CV_8U type, e.g., grayscale or BGR).
 * @param value The value to add to each pixel intensity. Positive values lighten,
 *              negative values darken.
 * @return cv::Mat The brightness-adjusted image.
 * @throws std::invalid_argument if the input image is empty.
 */
cv::Mat adjust_brightness(const cv::Mat& input_image, int value);

#endif // AI_SLOP_BRIGHTNESS_HPP 