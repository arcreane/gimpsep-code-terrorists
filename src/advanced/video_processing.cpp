#include "video_processing.hpp"
#include <iostream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp>  // Add this for BackgroundSubtractorMOG2

namespace ai_slop {

VideoProcessor::VideoProcessor(const std::string& input_path, const std::string& output_path)
    : input_path_(input_path)
    , output_path_(output_path)
    , frame_width_(0)
    , frame_height_(0)
    , fps_(0) {
    initialize();
}

void VideoProcessor::initialize() {
    // Open input video
    cap_.open(input_path_);
    if (!cap_.isOpened()) {
        throw std::runtime_error("Failed to open input video: " + input_path_);
    }

    // Get video properties
    frame_width_ = static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_WIDTH));
    frame_height_ = static_cast<int>(cap_.get(cv::CAP_PROP_FRAME_HEIGHT));
    fps_ = cap_.get(cv::CAP_PROP_FPS);

    // Initialize video writer
    int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
    writer_.open(output_path_, fourcc, fps_, cv::Size(frame_width_, frame_height_));
    if (!writer_.isOpened()) {
        throw std::runtime_error("Failed to create output video: " + output_path_);
    }
}

void VideoProcessor::cleanup() {
    if (cap_.isOpened()) {
        cap_.release();
    }
    if (writer_.isOpened()) {
        writer_.release();
    }
}

bool VideoProcessor::process(AdvancedProcessor& processor) {
    cv::Mat frame;
    while (cap_.read(frame)) {
        try {
            // Process the frame
            cv::Mat processed = processor.process(frame);
            
            // Write the processed frame
            writer_.write(processed);
        } catch (const std::exception& e) {
            cleanup();
            throw std::runtime_error("Error processing video frame: " + std::string(e.what()));
        }
    }

    cleanup();
    return true;
}

void process_video_grayscale(const std::string& input_path, const std::string& output_path) {
    // Open the video file
    cv::VideoCapture cap(input_path);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open video file: " + input_path);
    }

    // Get video properties
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);

    // Create video writer
    cv::VideoWriter writer(output_path, 
                          cv::VideoWriter::fourcc('m', 'p', '4', 'v'),
                          fps, 
                          cv::Size(frame_width, frame_height),
                          false); // false for grayscale

    if (!writer.isOpened()) {
        throw std::runtime_error("Error: Could not create output video file: " + output_path);
    }

    cv::Mat frame;
    while (cap.read(frame)) {
        // Convert to grayscale
        cv::Mat gray_frame;
        cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);
        
        // Write the frame
        writer.write(gray_frame);
    }

    // Release resources
    cap.release();
    writer.release();
}

bool process_video_bg_subtract_mog2(const std::string& input_video_path,
                                     const std::string& output_video_path,
                                     int history,
                                     double var_threshold,
                                     bool detect_shadows)
{
    // 1. Open the input video file
    cv::VideoCapture cap(input_video_path);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open input video file: " + input_video_path);
    }

    // 2. Get video properties (same as before)
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);
    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    // 3. Create the MOG2 background subtractor
    cv::Ptr<cv::BackgroundSubtractorMOG2> p_mog2 = cv::createBackgroundSubtractorMOG2(history, var_threshold, detect_shadows);

    // 4. Create the output video writer (for the foreground mask - single channel)
    cv::VideoWriter writer(output_video_path, fourcc, fps, cv::Size(frame_width, frame_height), false); // isColor = false for mask
    if (!writer.isOpened()) {
        cap.release();
        throw std::runtime_error("Error: Could not create output video file: " + output_video_path);
    }

    std::cout << "Processing video for background subtraction (MOG2): " << input_video_path << std::endl;
    std::cout << "  Saving foreground mask to: " << output_video_path << std::endl;

    // 5. Process frame by frame
    cv::Mat frame;
    cv::Mat fg_mask;
    int frame_count = 0;
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }

        // Apply the background subtractor
        // The learning rate can be specified, -1 uses the default internal rate
        p_mog2->apply(frame, fg_mask, -1);

        // fg_mask contains the foreground mask (0 for background, 255 for foreground, 127 for shadows if detect_shadows is true)
        writer.write(fg_mask);

        frame_count++;
        if (frame_count % 100 == 0) {
            std::cout << "Processed " << frame_count << " frames..." << std::endl;
        }
    }

    std::cout << "Finished processing " << frame_count << " frames." << std::endl;

    // 6. Release resources
    cap.release();
    writer.release();

    return true;
}

} // namespace ai_slop 