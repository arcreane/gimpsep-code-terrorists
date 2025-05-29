#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Include core processor headers
#include "core/image_processor.hpp"
#include "core/brightness_processor.hpp"
#include "core/morphology_processor.hpp"
#include "core/resize_processor.hpp"
#include "core/canny_processor.hpp"
#include "core/stitch_processor.hpp"

// Include advanced function headers
#include "advanced/video_processing.hpp"
#include "advanced/face_detection.hpp"
#include "advanced/object_detection.hpp"
#include "advanced/inpainting.hpp"

// Include CLI interface
#include "cli/menu_interface.hpp"

using ai_slop::BrightnessProcessor;
using ai_slop::MorphologyProcessor;
using ai_slop::ResizeProcessor;
using ai_slop::CannyProcessor;
using ai_slop::StitchProcessor;
using ai_slop::MenuInterface;

/**
 * @brief Main entry point for the AI_SLOP application.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 * @return int Exit code (0 for success, 1 for errors).
 */
int main(int argc, char** argv) {
    try {
        MenuInterface menu;
        menu.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
        return 1;
    }
    return 0;
} 