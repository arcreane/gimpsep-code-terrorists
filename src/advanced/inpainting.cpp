#include "inpainting.hpp"
#include <stdexcept> // For std::invalid_argument
#include <iostream>  // For status messages

cv::Mat inpaint_image(const cv::Mat& input_image,
                      const cv::Mat& mask_image,
                      double inpaint_radius,
                      int method)
{
    // 1. Input Validation
    if (input_image.empty()) {
        throw std::invalid_argument("Input image for inpainting is empty.");
    }
    if (mask_image.empty()) {
        throw std::invalid_argument("Mask image for inpainting is empty.");
    }
    if (input_image.size() != mask_image.size()) {
        throw std::invalid_argument("Input image and mask image must have the same dimensions.");
    }
    if (mask_image.type() != CV_8UC1) {
        throw std::invalid_argument("Mask image must be an 8-bit single-channel image (CV_8UC1).");
    }
    if (!(input_image.type() == CV_8UC1 || input_image.type() == CV_8UC3)) {
         throw std::invalid_argument("Input image must be 8-bit single-channel (CV_8UC1) or 3-channel (CV_8UC3).");
    }
    if (inpaint_radius <= 0) {
        throw std::invalid_argument("Inpaint radius must be positive.");
    }
    if (method != cv::INPAINT_NS && method != cv::INPAINT_TELEA) {
         throw std::invalid_argument("Invalid inpainting method specified.");
    }

    std::cout << "Performing inpainting..." << std::endl;
    std::cout << "  Radius: " << inpaint_radius << std::endl;
    std::cout << "  Method: " << (method == cv::INPAINT_NS ? "Navier-Stokes" : "TELEA") << std::endl;

    // 2. Perform Inpainting
    cv::Mat inpainted_image;
    cv::inpaint(input_image, mask_image, inpainted_image, inpaint_radius, method);

    std::cout << "Inpainting completed." << std::endl;

    return inpainted_image;
} 