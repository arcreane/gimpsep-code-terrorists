#include "resize_processor.hpp"
#include <opencv2/imgproc.hpp>
#include <stdexcept>

namespace ai_slop {

ResizeProcessor::ResizeProcessor(double factor) : factor_(factor) {
    if (factor <= 0)
        throw std::invalid_argument("Resize factor must be positive");
}

cv::Mat ResizeProcessor::process(const cv::Mat& input) {
    validateInput(input);
    cv::Mat output;
    cv::resize(input, output, cv::Size(), factor_, factor_, cv::INTER_LINEAR);
    return output;
}

std::string ResizeProcessor::getName() const {
    return "ResizeProcessor";
}

} // namespace ai_slop 