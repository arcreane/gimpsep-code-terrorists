#ifndef AI_SLOP_CANNY_HPP
#define AI_SLOP_CANNY_HPP

#include <opencv2/core.hpp> // For cv::Mat
#include <opencv2/imgproc.hpp> // For cv::Canny

/**
 * @brief Detects edges in an input image using the Canny algorithm.
 *
 * @param input_image The source image (cv::Mat). It's recommended to use a grayscale image,
 *                    although Canny can process multi-channel images (often treating them channel-wise).
 *                    This implementation will convert to grayscale first for standard behavior.
 * @param threshold1 The first threshold for the hysteresis procedure.
 * @param threshold2 The second threshold for the hysteresis procedure.
 * @param aperture_size Aperture size for the Sobel operator (default: 3).
 * @param l2_gradient A flag indicating whether to use a more accurate L2 norm
 *                    for calculating image gradient magnitude (true) or the default L1 norm (false).
 *                    Default is false for performance.
 * @return cv::Mat The output edge map (single-channel 8-bit image).
 * @throws std::invalid_argument if the input image is empty or thresholds are negative.
 */
cv::Mat detect_edges_canny(const cv::Mat& input_image,
                           double threshold1,
                           double threshold2,
                           int aperture_size = 3,
                           bool l2_gradient = false);

#endif // AI_SLOP_CANNY_HPP 