#include "main_window.hpp"
#include <iostream>

namespace ai_slop {

bool MainWindow::want_gui(int argc, char** argv) {
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if (arg == "--gui") return true;
    }
    return false;
}

MainWindow::MainWindow(
    const std::string& image_path,
    const std::string& video_path,
    const std::string& yolo_cfg,
    const std::string& yolo_weights,
    const std::string& yolo_names,
    const std::string& face_cascade
) :
    current_image_(cv::imread(image_path)),
    current_video_path_(video_path),
    is_video_mode_(false),
    object_detector_(yolo_cfg, yolo_weights, yolo_names),
    face_detector_(face_cascade),
    video_processor_(video_path, "output_video.mp4"),
    brightness_processor_(50),
    resize_processor_(1.5),
    morphology_processor_(MorphologyProcessor::Dilate, 3),
    canny_processor_(100, 200)
{
    if (current_image_.empty()) {
        std::cerr << "Warning: Could not load default image at " << image_path << std::endl;
        // Load a default image if none is provided
        current_image_ = cv::Mat::zeros(480, 640, CV_8UC3);
        cv::putText(current_image_, "No image loaded", cv::Point(50, 240), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);
    }
}

void MainWindow::run() {
    cv::namedWindow("AI_SLOP GUI", cv::WINDOW_NORMAL);
    cv::resizeWindow("AI_SLOP GUI", 1200, 600);

    int operation = 0;
    cv::createTrackbar("Operation (0:Obj,1:Face,2:Bright,3:Resize,4:Morph,5:Canny)", "AI_SLOP GUI", &operation, 5);

    int last_operation = -1;
    while (true) {
        if (operation != last_operation) {
            processImage(operation);
            last_operation = operation;
        }
        int key = cv::waitKey(30);
        if (key == 27) break; // ESC
    }
}

void MainWindow::processImage(int operation) {
    if (current_image_.empty()) return;
    cv::Mat processed;
    switch (operation) {
        case 0:
            processed = object_detector_.process(current_image_);
            break;
        case 1:
            processed = face_detector_.process(current_image_);
            break;
        case 2:
            processed = brightness_processor_.process(current_image_);
            break;
        case 3:
            processed = resize_processor_.process(current_image_);
            break;
        case 4:
            processed = morphology_processor_.process(current_image_);
            break;
        case 5:
            processed = canny_processor_.process(current_image_);
            break;
        default:
            processed = current_image_.clone();
    }
    displayImage(processed);
}

void MainWindow::displayImage(const cv::Mat& image) {
    cv::Mat combined;
    cv::hconcat(current_image_, image, combined);
    cv::imshow("AI_SLOP GUI", combined);
}

void MainWindow::displayVideo(const std::string& video_path) {
    cv::VideoCapture cap(video_path);
    if (!cap.isOpened()) {
        std::cerr << "Error: Could not open video." << std::endl;
        return;
    }
    cv::Mat frame;
    while (cap.read(frame)) {
        cv::imshow("AI_SLOP GUI", frame);
        if (cv::waitKey(30) == 27) break; // ESC
    }
}

} // namespace ai_slop 