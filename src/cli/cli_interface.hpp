#pragma once

#include "cli_parser.hpp"
#include "core/morphology_processor.hpp"
#include "core/brightness_processor.hpp"
#include "core/resize_processor.hpp"
#include "core/canny_processor.hpp"
#include "core/stitch_processor.hpp"
#include <string>
#include <vector>

namespace ai_slop {

class CLIInterface {
public:
    void run();

private:
    void handleDilation();
    void handleErosion();
    void handleResize();
    void handleBrightness();
    void handleCanny();
    void handleStitch();

    std::string getInputImagePath();
    std::vector<std::string> getMultipleInputPaths();
    std::string getOutputPath(const std::string& operation);
    int getKernelSize();
    double getResizeFactor();
    int getBrightnessValue();
    double getCannyThreshold1();
    double getCannyThreshold2();
    void processImage(const ParsedArguments& args);

    std::string prependInputFolder(const std::string& filename);
    std::string prependOutputFolder(const std::string& filename);
};

} // namespace ai_slop 