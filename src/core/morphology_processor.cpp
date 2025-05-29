#include "morphology_processor.hpp"
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <iostream>

namespace ai_slop {

MorphologyProcessor::MorphologyProcessor(Type type, int kernel_size)
    : type_(type), kernel_size_(kernel_size) {
    if (kernel_size < 1 || kernel_size % 2 == 0)
        throw std::invalid_argument("Kernel size must be a positive odd integer");
}

cv::Mat MorphologyProcessor::process(const cv::Mat& input) {
    validateInput(input);
    cv::Mat output;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, {kernel_size_, kernel_size_});
    if (type_ == Dilate)
        cv::dilate(input, output, kernel);
    else
        cv::erode(input, output, kernel);
    // Debug: print output image size
    std::cout << "[DEBUG] MorphologyProcessor output size: " << output.size() << std::endl;
    return output;
}

std::string MorphologyProcessor::getName() const {
    return (type_ == Dilate) ? "MorphologyProcessor(Dilate)" : "MorphologyProcessor(Erode)";
}

} // namespace ai_slop 