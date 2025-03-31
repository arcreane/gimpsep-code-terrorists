#include "video_processing.hpp"
#include <iostream>
#include <stdexcept>

bool process_video_grayscale(const std::string& input_video_path, const std::string& output_video_path) {
    // 1. Open the input video file
    cv::VideoCapture cap(input_video_path);
    if (!cap.isOpened()) {
        throw std::runtime_error("Error: Could not open input video file: " + input_video_path);
    }

    // 2. Get video properties
    int frame_width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    int frame_height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    double fps = cap.get(cv::CAP_PROP_FPS);
    int fourcc = cv::VideoWriter::fourcc('M', 'J', 'P', 'G'); // Or use another codec like MP4V, DIVX, etc.
                                                          // MJPG is often a good default for AVI.
                                                          // Use cap.get(cv::CAP_PROP_FOURCC) if you want to try preserving the original codec

    // 3. Create the output video writer
    cv::VideoWriter writer(output_video_path, fourcc, fps, cv::Size(frame_width, frame_height), false); // false for isColor (grayscale output)
    if (!writer.isOpened()) {
        // Clean up capture before throwing
        cap.release(); 
        throw std::runtime_error("Error: Could not create output video file: " + output_video_path);
    }

    std::cout << "Processing video: " << input_video_path << std::endl;
    std::cout << "  Resolution: " << frame_width << "x" << frame_height << std::endl;
    std::cout << "  FPS: " << fps << std::endl;
    std::cout << "Saving grayscale video to: " << output_video_path << std::endl;

    // 4. Process frame by frame
    cv::Mat frame;
    cv::Mat gray_frame;
    int frame_count = 0;
    while (true) {
        cap >> frame; // Read the next frame

        if (frame.empty()) {
            break; // End of video
        }

        // Process the frame (convert to grayscale)
        cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY);

        // Write the processed frame to the output video
        writer.write(gray_frame);

        frame_count++;
        if (frame_count % 100 == 0) { // Print progress periodically
            std::cout << "Processed " << frame_count << " frames..." << std::endl;
        }
    }

    std::cout << "Finished processing " << frame_count << " frames." << std::endl;

    // 5. Release resources
    cap.release();
    writer.release();

    return true;
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