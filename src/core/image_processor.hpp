#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <memory>

namespace ai_slop {

/**
 * @brief Base class for all image processing operations
 * 
 * This abstract class defines the interface that all image processors must implement.
 * It provides common functionality and enforces a consistent interface across all processors.
 */
class ImageProcessor {
public:
    virtual ~ImageProcessor() = default;

    /**
     * @brief Process an input image and return the result
     * 
     * @param input The input image to process
     * @return cv::Mat The processed image
     * @throws std::runtime_error if processing fails
     */
    virtual cv::Mat process(const cv::Mat& input) = 0;

    /**
     * @brief Get the name of the processor
     * 
     * @return std::string The processor's name
     */
    virtual std::string getName() const = 0;

protected:
    /**
     * @brief Validate the input image
     * 
     * @param input The input image to validate
     * @throws std::invalid_argument if the input is invalid
     */
    void validateInput(const cv::Mat& input) const {
        if (input.empty()) {
            throw std::invalid_argument("Input image is empty");
        }
    }

    /**
     * @brief Validate that the input image has the expected number of channels
     * 
     * @param input The input image to validate
     * @param expected_channels The expected number of channels
     * @throws std::invalid_argument if the input has the wrong number of channels
     */
    void validateChannels(const cv::Mat& input, int expected_channels) const {
        if (input.channels() != expected_channels) {
            throw std::invalid_argument("Input image has " + 
                std::to_string(input.channels()) + " channels, expected " + 
                std::to_string(expected_channels));
        }
    }
};

} // namespace ai_slop 