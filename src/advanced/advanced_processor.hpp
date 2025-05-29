#ifndef AI_SLOP_ADVANCED_PROCESSOR_HPP
#define AI_SLOP_ADVANCED_PROCESSOR_HPP

#include <opencv2/core.hpp>
#include <string>

namespace ai_slop {

/**
 * @brief Base class for advanced image processing operations
 */
class AdvancedProcessor {
public:
    virtual ~AdvancedProcessor() = default;

    /**
     * @brief Process the input image
     * @param input The input image to process
     * @return The processed image
     */
    virtual cv::Mat process(const cv::Mat& input) = 0;

    /**
     * @brief Get the name of the processor
     * @return The processor name
     */
    virtual std::string getName() const = 0;

protected:
    // Common utility functions can be added here
    void validateInput(const cv::Mat& input) const {
        if (input.empty()) {
            throw std::invalid_argument("Input image is empty");
        }
    }
};

} // namespace ai_slop

#endif // AI_SLOP_ADVANCED_PROCESSOR_HPP 