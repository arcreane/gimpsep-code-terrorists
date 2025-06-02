#ifndef AI_SLOP_STITCHING_HPP
#define AI_SLOP_STITCHING_HPP

#include <vector>
#include <string>
#include <opencv2/core.hpp> // For cv::Mat
#include <opencv2/stitching.hpp> // For cv::Stitcher

/**
 * @brief Attempts to stitch multiple input images into a panorama.
 *
 * This function takes a list of image file paths, loads them, and uses
 * OpenCV's Stitcher class to create a panorama.
 *
 * @param image_paths A vector of strings containing the paths to the input images.
 * @param output_pano Reference to a cv::Mat where the resulting panorama will be stored.
 * @return cv::Stitcher::Status The status code indicating the success or failure of the stitching process.
 *         (cv::Stitcher::OK indicates success).
 * @throws std::runtime_error if fewer than two image paths are provided or if images cannot be loaded.
 */
cv::Stitcher::Status stitch_images(const std::vector<std::string>& image_paths, cv::Mat& output_pano);


/**
 * @brief Converts a Stitcher status code to a human-readable string.
 *
 * @param status The status code returned by the stitcher.
 * @return std::string A descriptive string for the status.
 */
std::string stitcher_status_to_string(cv::Stitcher::Status status);


#endif // AI_SLOP_STITCHING_HPP 