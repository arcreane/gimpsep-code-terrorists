#ifndef AI_SLOP_OBJECT_DETECTION_HPP
#define AI_SLOP_OBJECT_DETECTION_HPP

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <opencv2/dnn.hpp>      // For cv::dnn::Net
#include <opencv2/imgproc.hpp> // For cv::rectangle, cv::putText etc.

/**
 * @brief Detects objects in an input image using a pre-trained YOLO (Darknet) model.
 *
 * Loads the YOLO network, processes the image, performs detection,
 * applies Non-Maximum Suppression (NMS), and draws bounding boxes with labels.
 *
 * @param input_image The source image (cv::Mat, BGR format expected).
 * @param config_path Path to the model configuration file (.cfg).
 * @param weights_path Path to the model weights file (.weights).
 * @param names_path Path to the file containing class names (one per line).
 * @param confidence_threshold Minimum confidence score to consider a detection valid.
 * @param nms_threshold Non-Maximum Suppression threshold to filter overlapping boxes.
 * @param input_width The width the input image is resized to before being fed to the network.
 * @param input_height The height the input image is resized to before being fed to the network.
 * @return cv::Mat A copy of the input image with bounding boxes and labels drawn.
 * @throws std::runtime_error if the input image is empty, model files cannot be loaded,
 *                            or class names cannot be read.
 */
cv::Mat detect_objects_yolo(const cv::Mat& input_image,
                              const std::string& config_path,
                              const std::string& weights_path,
                              const std::string& names_path,
                              float confidence_threshold = 0.5f,
                              float nms_threshold = 0.4f,
                              int input_width = 416, // Common YOLO input size
                              int input_height = 416);

#endif // AI_SLOP_OBJECT_DETECTION_HPP 