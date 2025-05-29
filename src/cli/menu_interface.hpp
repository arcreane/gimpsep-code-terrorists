#pragma once

#include <string>
#include <vector>
#include <functional>
#include <map>
#include <memory>
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace ai_slop {

/**
 * @brief Structure to hold operation parameters
 */
struct OperationParams {
    std::string input_path;
    std::string output_path;
    std::map<std::string, std::string> options;
};

/**
 * @brief Menu-based CLI interface for AI_SLOP
 */
class MenuInterface {
public:
    MenuInterface();
    void run();

private:
    // Menu display functions
    void showMainMenu();
    void showOperationMenu(const std::string& operation);
    void showHelp(const std::string& operation = "");

    // Input handling functions
    std::string getInputPath();
    std::string getOutputPath();
    int getIntInput(const std::string& prompt, int min, int max);
    double getDoubleInput(const std::string& prompt, double min, double max);
    std::string getStringInput(const std::string& prompt);

    // Operation parameter collection
    OperationParams collectBasicParams();
    OperationParams collectMorphologyParams();
    OperationParams collectResizeParams();
    OperationParams collectBrightnessParams();
    OperationParams collectCannyParams();
    OperationParams collectStitchParams();
    OperationParams collectFaceDetectionParams();

    // Operation execution
    void executeOperation(const std::string& operation, const OperationParams& params);
    void processImage(const std::string& operation, const OperationParams& params);

    // Helper functions
    bool validateInputPath(const std::string& path);
    bool validateOutputPath(const std::string& path);
    void printError(const std::string& message);
    void printSuccess(const std::string& message);
    void printProgress(const std::string& message);

    // Menu state
    bool running;
    std::map<std::string, std::function<void()>> main_menu_actions;
    std::map<std::string, std::string> operation_descriptions;
};

} // namespace ai_slop 