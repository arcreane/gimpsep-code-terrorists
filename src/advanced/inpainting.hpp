#ifndef AI_SLOP_INPAINTING_HPP
#define AI_SLOP_INPAINTING_HPP

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/photo.hpp> // For cv::inpaint
#include "advanced_processor.hpp"

namespace ai_slop {

/**
 * @brief Image inpainting processor
 */
class Inpainter : public AdvancedProcessor {
public:
    /**
     * @brief Construct a new Inpainter
     * @param inpaint_radius Radius of a circular neighborhood of each point inpainted
     * @param method Inpainting method (cv::INPAINT_NS or cv::INPAINT_TELEA)
     */
    Inpainter(double inpaint_radius = 3.0,
              int method = cv::INPAINT_NS);

    /**
     * @brief Process the input image to perform inpainting
     * @param input The input image to process
     * @param mask The inpainting mask (non-zero pixels indicate areas to inpaint)
     * @return The inpainted image
     */
    cv::Mat process(const cv::Mat& input, const cv::Mat& mask);

    /**
     * @brief Process the input image (required by base class)
     * @param input The input image to process
     * @return The processed image
     * @throws std::runtime_error if no mask is provided
     */
    cv::Mat process(const cv::Mat& input) override {
        throw std::runtime_error("Inpainter requires a mask image");
    }

    /**
     * @brief Get the name of the processor
     * @return The processor name
     */
    std::string getName() const override { return "Inpainter"; }

private:
    double inpaint_radius_;
    int method_;
};

} // namespace ai_slop

#endif // AI_SLOP_INPAINTING_HPP 