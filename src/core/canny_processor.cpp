#include "canny_processor.hpp"
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <iostream>

namespace ai_slop {

CannyProcessor::CannyProcessor(double threshold1, double threshold2)
    : threshold1_(threshold1), threshold2_(threshold2) {
    if (threshold1 < 0 || threshold2 < 0)
        throw std::invalid_argument("Canny thresholds must be non-negative");
}

cv::Mat CannyProcessor::process(const cv::Mat& input) {
    validateInput(input);
    std::cout << "[DEBUG] CannyProcessor: Input image size: " << input.size() << ", channels: " << input.channels() << std::endl;
    
    // Convert to grayscale if needed
    cv::Mat gray;
    if (input.channels() > 1) {
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        std::cout << "[DEBUG] CannyProcessor: Converted to grayscale, size: " << gray.size() << ", channels: " << gray.channels() << std::endl;
    } else {
        gray = input;
        std::cout << "[DEBUG] CannyProcessor: Already grayscale, size: " << gray.size() << ", channels: " << gray.channels() << std::endl;
    }
    
    // Apply Canny edge detection
    cv::Mat edges;
    cv::Canny(gray, edges, threshold1_, threshold2_);
    std::cout << "[DEBUG] CannyProcessor: After Canny, size: " << edges.size() << ", channels: " << edges.channels() << std::endl;
    
    // Convert single-channel edge map to 3-channel image
    cv::Mat output;
    cv::cvtColor(edges, output, cv::COLOR_GRAY2BGR);
    std::cout << "[DEBUG] CannyProcessor: Final output size: " << output.size() << ", channels: " << output.channels() << std::endl;
    
    return output;
}

std::string CannyProcessor::getName() const {
    return "CannyProcessor";
}

} // namespace ai_slop 