#ifndef AI_SLOP_FACE_DETECTION_HPP
#define AI_SLOP_FACE_DETECTION_HPP

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp> // For cv::CascadeClassifier
#include <opencv2/imgproc.hpp> // For cv::rectangle, cv::cvtColor
#include "advanced_processor.hpp"

namespace ai_slop {

/**
 * @brief Face detection processor using Haar cascade classifier
 */
class FaceDetector : public AdvancedProcessor {
public:
    /**
     * @brief Construct a new Face Detector
     * @param cascade_file_path Path to the Haar cascade XML file
     * @param scale_factor Parameter specifying how much the image size is reduced at each image scale
     * @param min_neighbors Parameter specifying how many neighbors each candidate rectangle should have
     * @param min_size Minimum possible object size
     */
    FaceDetector(const std::string& cascade_file_path,
                 double scale_factor = 1.1,
                 int min_neighbors = 3,
                 cv::Size min_size = cv::Size(30, 30));

    /**
     * @brief Process the input image to detect faces
     * @param input The input image to process
     * @return The image with rectangles drawn around detected faces
     */
    cv::Mat process(const cv::Mat& input) override;

    /**
     * @brief Get the name of the processor
     * @return The processor name
     */
    std::string getName() const override { return "FaceDetector"; }

private:
    cv::CascadeClassifier classifier_;
    double scale_factor_;
    int min_neighbors_;
    cv::Size min_size_;
};

} // namespace ai_slop

#endif // AI_SLOP_FACE_DETECTION_HPP 