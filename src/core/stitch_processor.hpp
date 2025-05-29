#pragma once

#include "image_processor.hpp"
#include <opencv2/core.hpp>
#include <string>
#include <vector>

namespace ai_slop {

class StitchProcessor : public ImageProcessor {
public:
    StitchProcessor(const std::vector<std::string>& input_files);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    std::vector<std::string> input_files_;
};

} // namespace ai_slop 