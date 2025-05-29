#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include "core/canny_processor.hpp"

int main() {
    try {
        // Load input image
        cv::Mat input_image = cv::imread("../input1.jpg");
        if (input_image.empty()) {
            throw std::runtime_error("Failed to load input image");
        }
        std::cout << "Input image loaded successfully. Size: " << input_image.size() << std::endl;

        // Create Canny processor with thresholds
        ai_slop::CannyProcessor processor(100.0, 200.0);
        
        // Process the image
        cv::Mat output_image = processor.process(input_image);
        
        // Save the result
        if (!cv::imwrite("../output_canny_direct.jpg", output_image)) {
            throw std::runtime_error("Failed to save output image");
        }
        
        std::cout << "Processing complete! Output saved to: output_canny_direct.jpg" << std::endl;
        
        // Show the result
        cv::imshow("Input", input_image);
        cv::imshow("Canny Edges", output_image);
        cv::waitKey(0);
        cv::destroyAllWindows();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
} 