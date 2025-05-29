#include "object_detection.hpp"
#include <fstream>   // For reading class names file
#include <stdexcept> // For exceptions
#include <iostream>  // For status messages
#include <vector>

namespace ai_slop {

ObjectDetector::ObjectDetector(const std::string& config_path,
                             const std::string& weights_path,
                             const std::string& names_path,
                             float confidence_threshold,
                             float nms_threshold,
                             int input_width,
                             int input_height)
    : confidence_threshold_(confidence_threshold)
    , nms_threshold_(nms_threshold)
    , input_width_(input_width)
    , input_height_(input_height) {
    
    // Load the network
    net_ = cv::dnn::readNetFromDarknet(config_path, weights_path);
    if (net_.empty()) {
        throw std::runtime_error("Failed to load YOLO network");
    }

    // Load class names
    loadClassNames(names_path);
}

void ObjectDetector::loadClassNames(const std::string& names_path) {
    std::ifstream file(names_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open class names file: " + names_path);
    }

    std::string line;
    while (std::getline(file, line)) {
        class_names_.push_back(line);
    }
}

std::vector<cv::Mat> ObjectDetector::preprocess(const cv::Mat& input) {
    // Create a blob from the image
    cv::Mat blob = cv::dnn::blobFromImage(input, 1/255.0, cv::Size(input_width_, input_height_),
                                        cv::Scalar(0,0,0), true, false);

    // Set the input to the network
    net_.setInput(blob);

    // Forward pass
    std::vector<cv::Mat> outputs;
    std::vector<std::string> outNames = net_.getUnconnectedOutLayersNames();
    net_.forward(outputs, outNames);

    return outputs;
}

std::vector<cv::Rect> ObjectDetector::postprocess(const std::vector<cv::Mat>& outputs,
                                                const cv::Size& original_size) {
    std::vector<cv::Rect> boxes;
    std::vector<float> confidences;
    std::vector<int> classIds;

    // Process each output layer
    for (const auto& output : outputs) {
        float* data = (float*)output.data;
        for (int i = 0; i < output.rows; ++i, data += output.cols) {
            cv::Mat scores = output.row(i).colRange(5, output.cols);
            cv::Point classIdPoint;
            double confidence;
            cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);

            if (confidence > confidence_threshold_) {
                int centerX = (int)(data[0] * original_size.width);
                int centerY = (int)(data[1] * original_size.height);
                int width = (int)(data[2] * original_size.width);
                int height = (int)(data[3] * original_size.height);
                int left = centerX - width / 2;
                int top = centerY - height / 2;

                boxes.push_back(cv::Rect(left, top, width, height));
                confidences.push_back((float)confidence);
                classIds.push_back(classIdPoint.x);
            }
        }
    }

    // Apply non-maximum suppression
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confidence_threshold_, nms_threshold_, indices);

    std::vector<cv::Rect> result;
    for (int idx : indices) {
        result.push_back(boxes[idx]);
    }

    return result;
}

cv::Mat ObjectDetector::process(const cv::Mat& input) {
    validateInput(input);

    // Get network outputs
    std::vector<cv::Mat> outputs = preprocess(input);

    // Process outputs to get bounding boxes
    std::vector<cv::Rect> boxes = postprocess(outputs, input.size());

    // Draw results
    cv::Mat result = input.clone();
    for (const auto& box : boxes) {
        cv::rectangle(result, box, cv::Scalar(0, 255, 0), 2);
        cv::putText(result, class_names_[box.x], cv::Point(box.x, box.y - 5),
                   cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }

    return result;
}

} // namespace ai_slop

// Helper function to get output layer names for YOLO
std::vector<cv::String> get_output_layer_names(const cv::dnn::Net& net) {
    std::vector<cv::String> names;
    std::vector<int> out_layers = net.getUnconnectedOutLayers();
    std::vector<cv::String> layers_names = net.getLayerNames();
    names.resize(out_layers.size());
    for (size_t i = 0; i < out_layers.size(); ++i)
        names[i] = layers_names[out_layers[i] - 1];
    return names;
}

cv::Mat detect_objects_yolo(const cv::Mat& input_image,
                              const std::string& config_path,
                              const std::string& weights_path,
                              const std::string& names_path,
                              float confidence_threshold,
                              float nms_threshold,
                              int input_width,
                              int input_height)
{
    if (input_image.empty()) {
        throw std::runtime_error("Input image for object detection is empty.");
    }

    // 1. Load class names
    std::vector<std::string> class_names;
    std::ifstream ifs(names_path.c_str());
    if (!ifs.is_open()) {
        throw std::runtime_error("Error opening class names file: " + names_path);
    }
    std::string line;
    while (std::getline(ifs, line)) {
        class_names.push_back(line);
    }
    ifs.close();
    if (class_names.empty()) {
         throw std::runtime_error("Class names file is empty or could not be read: " + names_path);
    }
    std::cout << "Loaded " << class_names.size() << " class names." << std::endl;

    // 2. Load the network
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(config_path, weights_path);
    if (net.empty()) {
         throw std::runtime_error("Failed to load YOLO model using config: " + config_path + " and weights: " + weights_path);
    }
    // Optional: Set preferable backend and target (e.g., for CUDA or OpenCL acceleration if available and OpenCV built with support)
    // net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    // net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
    // Or use OpenCL: DNN_BACKEND_OPENCV, DNN_TARGET_OPENCL
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    std::cout << "Loaded YOLO network successfully." << std::endl;

    // 3. Create input blob
    cv::Mat blob;
    // Size: Target network input size
    // Scalar: Mean subtraction values (often 0 for YOLO)
    // Bool swapRB: Swap Red and Blue channels (Darknet models expect BGR)
    // Bool crop: Whether to crop after resize (false)
    cv::dnn::blobFromImage(input_image, blob, 1.0/255.0, cv::Size(input_width, input_height), true, false);

    // 4. Set input
    net.setInput(blob);

    // 5. Forward pass
    std::vector<cv::Mat> outputs;
    std::vector<cv::String> output_names = get_output_layer_names(net);
    net.forward(outputs, output_names); // Get output from specified layers
    std::cout << "Network forward pass completed. Output tensors: " << outputs.size() << std::endl;

    // 6. Process outputs and prepare for NMS
    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    float x_factor = static_cast<float>(input_image.cols) / input_width;
    float y_factor = static_cast<float>(input_image.rows) / input_height;

    for (const auto& output : outputs) {
        // Each output is a Mat with rows = number of detections, cols = 4 (bbox) + 1 (obj conf) + num_classes
        const float* data = (float*)output.data;
        for (int i = 0; i < output.rows; ++i, data += output.cols) {
            cv::Mat scores = output.row(i).colRange(5, output.cols);
            cv::Point class_id_point;
            double confidence;
            // Find the class with the highest score
            cv::minMaxLoc(scores, 0, &confidence, 0, &class_id_point);

            if (confidence > confidence_threshold) {
                float center_x = data[0] * x_factor;
                float center_y = data[1] * y_factor;
                float width = data[2] * x_factor;
                float height = data[3] * y_factor;
                int left = static_cast<int>(center_x - width / 2);
                int top = static_cast<int>(center_y - height / 2);

                class_ids.push_back(class_id_point.x);
                confidences.push_back((float)confidence);
                boxes.push_back(cv::Rect(left, top, static_cast<int>(width), static_cast<int>(height)));
            }
        }
    }
     std::cout << "Processed outputs. Initial detections: " << boxes.size() << std::endl;

    // 7. Apply Non-Maximum Suppression (NMS)
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confidence_threshold, nms_threshold, indices);
    std::cout << "Applied NMS. Final detections: " << indices.size() << std::endl;

    // 8. Draw final bounding boxes and labels
    cv::Mat output_image = input_image.clone();
    for (int idx : indices) {
        cv::Rect box = boxes[idx];
        int class_id = class_ids[idx];
        float confidence = confidences[idx];

        // Draw rectangle
        cv::rectangle(output_image, box, cv::Scalar(0, 255, 0), 2);

        // Prepare label text
        std::string label = class_names[class_id] + ": " + cv::format("%.2f", confidence);

        // Get text size
        int base_line;
        cv::Size label_size = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &base_line);

        // Draw filled rectangle background for label
        cv::rectangle(output_image,
                      cv::Point(box.x, box.y - label_size.height - base_line),
                      cv::Point(box.x + label_size.width, box.y),
                      cv::Scalar(0, 255, 0), // Background color (Green)
                      cv::FILLED);

        // Draw label text
        cv::putText(output_image, label,
                    cv::Point(box.x, box.y - base_line),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1); // Text color (Black)
    }

    return output_image;
} 