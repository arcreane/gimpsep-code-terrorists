#ifndef AI_SLOP_INPAINTING_HPP
#define AI_SLOP_INPAINTING_HPP

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/photo.hpp> // For cv::inpaint

/**
 * @brief Performs image inpainting to fill masked regions.
 *
 * Uses the algorithm specified (default: Navier-Stokes based method) to fill
 * the areas indicated by non-zero pixels in the mask image.
 *
 * @param input_image The source image (cv::Mat, 8-bit 1-channel or 3-channel).
 * @param mask_image The inpainting mask (cv::Mat, 8-bit 1-channel). 
 *                   Non-zero pixels indicate the area to be inpainted.
 * @param inpaint_radius Radius of a circular neighborhood of each point inpainted.
 * @param method Inpainting method. cv::INPAINT_NS (Navier-Stokes) or cv::INPAINT_TELEA.
 * @return cv::Mat The inpainted image.
 * @throws std::invalid_argument if input images are empty, have wrong types/sizes, or invalid radius.
 */
cv::Mat inpaint_image(const cv::Mat& input_image,
                      const cv::Mat& mask_image,
                      double inpaint_radius = 3.0, // Default radius
                      int method = cv::INPAINT_NS); // Default method

#endif // AI_SLOP_INPAINTING_HPP 