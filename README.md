# AI_SLOP - Advanced Image Processing Library

AI_SLOP is a C++ library for advanced image processing operations, built on top of OpenCV. It provides a clean, object-oriented interface for various image processing tasks.

## Features

### Core Image Processing
- Brightness adjustment
- Morphological operations (dilation, erosion)
- Image resizing
- Canny edge detection
- Image stitching

### Advanced Features
- Face detection using Haar cascades
- Object detection using YOLO
- Video processing
- Image inpainting

## Requirements

- C++17 or later
- OpenCV 4.x
- CMake 3.10 or later

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Installation

```bash
cd build
sudo make install
```

## Usage

### Command Line Interface

```bash
# Basic usage
./AI_SLOP <operation> <input_image> <output_image>

# Examples
./AI_SLOP brightness input.jpg output.jpg
./AI_SLOP canny input.jpg output.jpg
./AI_SLOP face input.jpg output.jpg
```

### As a Library

```cpp
#include <ai_slop/core/image_processor.hpp>
#include <ai_slop/core/brightness_processor.hpp>

// Create a processor
ai_slop::BrightnessProcessor processor(50);

// Process an image
cv::Mat result = processor.process(input_image);
```

## Project Structure

```
AI_SLOP/
├── src/
│   ├── core/           # Core image processing classes
│   ├── advanced/       # Advanced features (face detection, etc.)
│   └── cli/           # Command-line interface
├── tests/             # Test programs
├── examples/          # Example code
└── docs/             # Documentation
```

## Contributing

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a new Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details. 