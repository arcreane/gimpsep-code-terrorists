#include "cli_parser.hpp"
#include <iostream>
#include <algorithm>

namespace ai_slop {

ParsedArguments parse_arguments(int argc, char** argv) {
    ParsedArguments args;
    
    if (argc < 2) {
        print_usage();
        args.show_help = true;
        return args;
    }

    // First argument should be the operation
    args.operation = argv[1];
    std::transform(args.operation.begin(), args.operation.end(), args.operation.begin(), ::tolower);

    // Parse remaining arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "-h" || arg == "--help") {
            print_usage();
            args.show_help = true;
            return args;
        }
        else if (arg == "-i" || arg == "--input") {
            if (i + 1 < argc) {
                args.input_files.push_back(argv[++i]);
            }
        }
        else if (arg == "-o" || arg == "--output") {
            if (i + 1 < argc) {
                args.output_file = argv[++i];
            }
        }
        else if (arg == "-k" || arg == "--kernel_size") {
            if (i + 1 < argc) {
                args.kernel_size = std::stoi(argv[++i]);
            }
        }
        else if (arg == "-f" || arg == "--factor") {
            if (i + 1 < argc) {
                args.resize_factor = std::stod(argv[++i]);
            }
        }
        else if (arg == "-b" || arg == "--brightness") {
            if (i + 1 < argc) {
                args.brightness_value = std::stoi(argv[++i]);
            }
        }
        else if (arg == "-t1" || arg == "--threshold1") {
            if (i + 1 < argc) {
                args.canny_threshold1 = std::stod(argv[++i]);
            }
        }
        else if (arg == "-t2" || arg == "--threshold2") {
            if (i + 1 < argc) {
                args.canny_threshold2 = std::stod(argv[++i]);
            }
        }
    }

    return args;
}

void print_usage() {
    std::cout << "Usage: AI_SLOP <operation> [options]\n\n"
              << "Operations:\n"
              << "  dilate     - Perform dilation on an image\n"
              << "  erode      - Perform erosion on an image\n"
              << "  resize     - Resize an image\n"
              << "  brightness - Adjust image brightness\n"
              << "  canny      - Perform Canny edge detection\n"
              << "  stitch     - Stitch multiple images together\n\n"
              << "Options:\n"
              << "  -h, --help           Show this help message\n"
              << "  -i, --input <file>   Input image file(s)\n"
              << "  -o, --output <file>  Output image file\n"
              << "  -k, --kernel_size <n> Kernel size for morphology operations\n"
              << "  -f, --factor <n>     Resize factor\n"
              << "  -b, --brightness <n> Brightness adjustment value (-255 to 255)\n"
              << "  -t1, --threshold1 <n> First threshold for Canny edge detection\n"
              << "  -t2, --threshold2 <n> Second threshold for Canny edge detection\n";
}

} // namespace ai_slop 