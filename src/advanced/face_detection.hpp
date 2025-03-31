#ifndef AI_SLOP_FACE_DETECTION_HPP
#define AI_SLOP_FACE_DETECTION_HPP

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/objdetect.hpp> // For cv::CascadeClassifier
#include <opencv2/imgproc.hpp> // For cv::rectangle, cv::cvtColor

/**
 * @brief Detects faces in an input image using a Haar cascade classifier.
 *
 * Loads a pre-trained Haar cascade XML file, converts the image to grayscale,
 * detects faces, and draws rectangles around them on the original image.
 *
 * @param input_image The source image (cv::Mat, BGR format expected).
 * @param cascade_file_path Path to the Haar cascade XML file (e.g., haarcascade_frontalface_default.xml).
 * @param scale_factor Parameter specifying how much the image size is reduced at each image scale.
 * @param min_neighbors Parameter specifying how many neighbors each candidate rectangle should have to retain it.
 * @param min_size Minimum possible object size. Objects smaller than this are ignored.
 * @return cv::Mat A copy of the input image with rectangles drawn around detected faces.
 * @throws std::runtime_error if the input image is empty or the cascade file cannot be loaded.
 */
cv::Mat detect_faces(const cv::Mat& input_image,
                     const std::string& cascade_file_path,
                     double scale_factor = 1.1,
                     int min_neighbors = 3,
                     cv::Size min_size = cv::Size(30, 30)); // Default min size

#endif // AI_SLOP_FACE_DETECTION_HPP 