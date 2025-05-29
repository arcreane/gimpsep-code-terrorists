#include "brightness_processor.hpp"
#include <stdexcept>

namespace ai_slop {

BrightnessProcessor::BrightnessProcessor(int brightness_value)
    : brightness_value_(brightness_value) {
    if (brightness_value < -255 || brightness_value > 255) {
        throw std::invalid_argument("Brightness value must be between -255 and 255");
    }
}

cv::Mat BrightnessProcessor::process(const cv::Mat& input) {
    validateInput(input);
    
    cv::Mat output;
    input.convertTo(output, -1, 1, brightness_value_);
    return output;
}

std::string BrightnessProcessor::getName() const {
    return "BrightnessProcessor";
}

} // namespace ai_slop 