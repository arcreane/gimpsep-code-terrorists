#include "stitch_processor.hpp"
#include <opencv2/stitching.hpp>
#include <opencv2/imgcodecs.hpp>
#include <stdexcept>

namespace ai_slop {

StitchProcessor::StitchProcessor(const std::vector<std::string>& input_files)
    : input_files_(input_files) {
    if (input_files.size() < 2)
        throw std::invalid_argument("Stitching requires at least two input images");
}

cv::Mat StitchProcessor::process(const cv::Mat& input) {
    validateInput(input);
    cv::Mat output;
    auto stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    std::vector<cv::Mat> images;
    for (const auto& file : input_files_) {
        cv::Mat img = cv::imread(file);
        if (img.empty())
            throw std::runtime_error("Failed to load image: " + file);
        images.push_back(img);
    }
    cv::Stitcher::Status status = stitcher->stitch(images, output);
    if (status != cv::Stitcher::OK)
        throw std::runtime_error("Stitching failed with status: " + std::to_string(status));
    return output;
}

std::string StitchProcessor::getName() const {
    return "StitchProcessor";
}

} // namespace ai_slop 