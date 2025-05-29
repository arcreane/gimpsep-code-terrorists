#pragma once

#include "image_processor.hpp"
#include <opencv2/core.hpp>
#include <string>

namespace ai_slop {

class MorphologyProcessor : public ImageProcessor {
public:
    enum Type { Dilate, Erode };
    MorphologyProcessor(Type type, int kernel_size);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    Type type_;
    int kernel_size_;
};

} // namespace ai_slop 