#ifndef AI_SLOP_VIDEO_PROCESSING_HPP
#define AI_SLOP_VIDEO_PROCESSING_HPP

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp> // For cv::VideoCapture, cv::VideoWriter
#include <opencv2/imgproc.hpp> // For cv::cvtColor etc.
#include <opencv2/video.hpp>   // For background subtraction (MOG2)

/**
 * @brief Processes an input video file, applies a grayscale filter to each frame,
 *        and saves the result to an output video file.
 *
 * @param input_video_path Path to the input video file.
 * @param output_video_path Path where the processed output video will be saved.
 * @return bool True if processing was successful and the video was saved, false otherwise.
 * @throws std::runtime_error if the input video cannot be opened or the output video cannot be created.
 */
bool process_video_grayscale(const std::string& input_video_path, const std::string& output_video_path);

/**
 * @brief Performs background subtraction on a video using the MOG2 algorithm.
 *
 * Reads an input video, applies the MOG2 background subtractor to each frame,
 * and saves the resulting foreground mask video.
 *
 * @param input_video_path Path to the input video file.
 * @param output_video_path Path where the foreground mask video will be saved.
 * @param history Length of the history for the MOG2 model.
 * @param var_threshold Threshold on the squared Mahalanobis distance to decide if a pixel is background.
 * @param detect_shadows If true, the algorithm will detect shadows and mark them differently (gray in the mask).
 * @return bool True if processing was successful, false otherwise.
 * @throws std::runtime_error if the input video cannot be opened or the output video cannot be created.
 */
bool process_video_bg_subtract_mog2(const std::string& input_video_path,
                                     const std::string& output_video_path,
                                     int history = 500,         // Default history
                                     double var_threshold = 16, // Default threshold
                                     bool detect_shadows = true); // Default detect shadows

// Add other video processing functions here later (e.g., applying different filters, stabilization, etc.)

#endif // AI_SLOP_VIDEO_PROCESSING_HPP 