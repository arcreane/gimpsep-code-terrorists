#ifndef AI_SLOP_OBJECT_DETECTION_HPP
#define AI_SLOP_OBJECT_DETECTION_HPP

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>      // For cv::dnn::Net
#include <opencv2/imgproc.hpp> // For cv::rectangle, cv::putText etc.
#include "advanced_processor.hpp"

namespace ai_slop {

/**
 * @brief Object detection processor using YOLO
 */
class ObjectDetector : public AdvancedProcessor {
public:
    /**
     * @brief Construct a new Object Detector
     * @param config_path Path to the model configuration file (.cfg)
     * @param weights_path Path to the model weights file (.weights)
     * @param names_path Path to the file containing class names
     * @param confidence_threshold Minimum confidence score for detections
     * @param nms_threshold Non-Maximum Suppression threshold
     * @param input_width Network input width
     * @param input_height Network input height
     */
    ObjectDetector(const std::string& config_path,
                  const std::string& weights_path,
                  const std::string& names_path,
                  float confidence_threshold = 0.5f,
                  float nms_threshold = 0.4f,
                  int input_width = 416,
                  int input_height = 416);

    /**
     * @brief Process the input image to detect objects
     * @param input The input image to process
     * @return The image with bounding boxes and labels drawn
     */
    cv::Mat process(const cv::Mat& input) override;

    /**
     * @brief Get the name of the processor
     * @return The processor name
     */
    std::string getName() const override { return "ObjectDetector"; }

private:
    cv::dnn::Net net_;
    std::vector<std::string> class_names_;
    float confidence_threshold_;
    float nms_threshold_;
    int input_width_;
    int input_height_;

    void loadClassNames(const std::string& names_path);
    std::vector<cv::Mat> preprocess(const cv::Mat& input);
    std::vector<cv::Rect> postprocess(const std::vector<cv::Mat>& outputs,
                                    const cv::Size& original_size);
};

} // namespace ai_slop

#endif // AI_SLOP_OBJECT_DETECTION_HPP 