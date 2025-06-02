#pragma once

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp> // For cv::VideoCapture, cv::VideoWriter
#include <opencv2/imgproc.hpp> // For cv::cvtColor etc.
#include "advanced_processor.hpp"

namespace ai_slop {

/**
 * @brief Video processing processor
 */
class VideoProcessor : public AdvancedProcessor {
public:
    /**
     * @brief Construct a new Video Processor
     * @param input_path Path to the input video file
     * @param output_path Path where the processed video will be saved
     */
    VideoProcessor(const std::string& input_path, const std::string& output_path);

    /**
     * @brief Process the input video
     * @param processor The image processor to apply to each frame
     * @return True if processing was successful
     */
    bool process(AdvancedProcessor& processor);

    /**
     * @brief Process the input image (required by base class)
     * @param input The input image to process
     * @return The processed image
     * @throws std::runtime_error as this is a video processor
     */
    cv::Mat process(const cv::Mat& input) override {
        throw std::runtime_error("VideoProcessor requires a processor to process frames");
    }

    /**
     * @brief Get the name of the processor
     * @return The processor name
     */
    std::string getName() const override { return "VideoProcessor"; }

    virtual ~VideoProcessor() = default;

private:
    std::string input_path_;
    std::string output_path_;
    cv::VideoCapture cap_;
    cv::VideoWriter writer_;
    int frame_width_;
    int frame_height_;
    double fps_;

    void initialize();
    void cleanup();
};

/**
 * @brief Processes an input video file, applies a grayscale filter to each frame,
 *        and saves the result to an output video file.
 *
 * @param input_video_path Path to the input video file.
 * @param output_video_path Path where the processed output video will be saved.
 * @return bool True if processing was successful and the video was saved, false otherwise.
 * @throws std::runtime_error if the input video cannot be opened or the output video cannot be created.
 */
void process_video_grayscale(const std::string& input_video_path, const std::string& output_video_path);

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

} // namespace ai_slop 