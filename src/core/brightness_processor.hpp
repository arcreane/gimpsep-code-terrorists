#pragma once

#include "image_processor.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace ai_slop {

/**
 * @brief Processor for adjusting image brightness
 * 
 * This processor adds a specified value to each pixel intensity.
 * Values are clamped to the valid range [0, 255] for standard 8-bit images.
 */
class BrightnessProcessor : public ImageProcessor {
public:
    /**
     * @brief Construct a new Brightness Processor
     * 
     * @param brightness_value The value to add to each pixel (-255 to 255)
     * @throws std::invalid_argument if brightness_value is out of range
     */
    explicit BrightnessProcessor(int brightness_value);

    /**
     * @brief Process the input image by adjusting its brightness
     * 
     * @param input The input image
     * @return cv::Mat The brightness-adjusted image
     * @throws std::runtime_error if processing fails
     */
    cv::Mat process(const cv::Mat& input) override;

    /**
     * @brief Get the name of the processor
     * 
     * @return std::string "BrightnessProcessor"
     */
    std::string getName() const override;

private:
    int brightness_value_;
};

} // namespace ai_slop 