#include "inpainting.hpp"
#include <stdexcept> // For std::invalid_argument
#include <iostream>  // For status messages

namespace ai_slop {

Inpainter::Inpainter(double inpaint_radius, int method)
    : inpaint_radius_(inpaint_radius)
    , method_(method) {
    if (inpaint_radius <= 0) {
        throw std::invalid_argument("Inpaint radius must be positive");
    }
    if (method != cv::INPAINT_NS && method != cv::INPAINT_TELEA) {
        throw std::invalid_argument("Invalid inpainting method");
    }
}

cv::Mat Inpainter::process(const cv::Mat& input, const cv::Mat& mask) {
    validateInput(input);
    
    if (mask.empty()) {
        throw std::invalid_argument("Mask image is empty");
    }
    if (mask.channels() != 1) {
        throw std::invalid_argument("Mask must be a single-channel image");
    }
    if (input.size() != mask.size()) {
        throw std::invalid_argument("Input and mask images must have the same size");
    }

    cv::Mat result;
    cv::inpaint(input, mask, result, inpaint_radius_, method_);
    return result;
}

} // namespace ai_slop 