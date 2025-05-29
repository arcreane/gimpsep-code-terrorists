#pragma once

#include "image_processor.hpp"
#include <opencv2/core.hpp>
#include <string>

namespace ai_slop {

class CannyProcessor : public ImageProcessor {
public:
    CannyProcessor(double threshold1, double threshold2);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    double threshold1_;
    double threshold2_;
};

} // namespace ai_slop 