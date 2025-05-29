#pragma once

#include "image_processor.hpp"
#include <opencv2/core.hpp>
#include <string>

namespace ai_slop {

class ResizeProcessor : public ImageProcessor {
public:
    ResizeProcessor(double factor);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    double factor_;
};

} // namespace ai_slop 