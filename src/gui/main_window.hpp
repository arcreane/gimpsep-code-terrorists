#ifndef AI_SLOP_MAIN_WINDOW_HPP
#define AI_SLOP_MAIN_WINDOW_HPP

#include <opencv2/opencv.hpp>
#include <string>
#include "advanced/object_detection.hpp"
#include "advanced/face_detection.hpp"
#include "advanced/video_processing.hpp"
#include "core/brightness_processor.hpp"
#include "core/resize_processor.hpp"
#include "core/morphology_processor.hpp"
#include "core/canny_processor.hpp"

namespace ai_slop {

class MainWindow {
public:
    MainWindow(
        const std::string& image_path = "images/input/input1.jpg",
        const std::string& video_path = "images/input/testvideo.mp4",
        const std::string& yolo_cfg = "models/yolo/yolov4-tiny.cfg",
        const std::string& yolo_weights = "models/yolo/yolov4-tiny.weights",
        const std::string& yolo_names = "models/yolo/coco.names",
        const std::string& face_cascade = "models/haarcascades/haarcascade_frontalface_default.xml"
    );
    void run();
    static bool want_gui(int argc, char** argv);

private:
    cv::Mat current_image_;
    std::string current_video_path_;
    bool is_video_mode_;

    // Processors
    ObjectDetector object_detector_;
    FaceDetector face_detector_;
    VideoProcessor video_processor_;
    BrightnessProcessor brightness_processor_;
    ResizeProcessor resize_processor_;
    MorphologyProcessor morphology_processor_;
    CannyProcessor canny_processor_;

    // GUI functions
    void processImage(int operation);
    void displayImage(const cv::Mat& image);
    void displayVideo(const std::string& video_path);
};

} // namespace ai_slop

#endif // AI_SLOP_MAIN_WINDOW_HPP 