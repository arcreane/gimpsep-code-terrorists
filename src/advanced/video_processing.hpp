#ifndef AI_SLOP_VIDEO_PROCESSING_HPP
#define AI_SLOP_VIDEO_PROCESSING_HPP

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp> // For cv::VideoCapture, cv::VideoWriter
#include <opencv2/imgproc.hpp> // For cv::cvtColor etc.

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

// Add other video processing functions here later (e.g., applying different filters, stabilization, etc.)

#endif // AI_SLOP_VIDEO_PROCESSING_HPP 