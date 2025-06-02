# AI_SLOP Image Processing Features Explanation

## 1. Resize Feature
The resize feature allows users to scale images up or down while maintaining aspect ratio and image quality.

### Implementation Details
```cpp
// src/core/resize_processor.hpp/cpp
class ResizeProcessor : public ImageProcessor {
    // Takes a resize factor (0.1-10.0)
    // Uses OpenCV's resize function with INTER_LINEAR interpolation
    // Maintains aspect ratio while scaling
```

### How it Works
- User provides a resize factor (e.g., 2.0 doubles the size, 0.5 halves it)
- Uses OpenCV's `resize` function with linear interpolation
- Preserves image quality while scaling
- Accessible through menu option 3 in the CLI
- Parameters are validated to ensure factor is between 0.1 and 10.0

## 2. Lighten/Darken (Brightness Adjustment)
The brightness adjustment feature allows users to modify the brightness of images, either making them lighter or darker.

### Implementation Details
```cpp
// src/core/brightness_processor.hpp/cpp
class BrightnessProcessor : public ImageProcessor {
    // Takes a brightness value (-255 to 255)
    // Negative values darken, positive values lighten
    // Uses OpenCV's convertTo for efficient processing
```

### How it Works
- User provides a brightness value (-255 to 255)
- Negative values darken the image
- Positive values lighten the image
- Uses OpenCV's `convertTo` for efficient processing
- Accessible through menu option 4 in the CLI
- Parameters are validated to ensure value is within range

## 3. Advanced Feature - Face Detection
The face detection feature uses computer vision techniques to identify and mark faces in images.

### Implementation Details
```cpp
// src/advanced/face_detection.hpp/cpp
class FaceDetector : public AdvancedProcessor {
    // Uses Haar Cascade Classifier
    // Parameters:
    // - Scale factor (1.1-2.0): How much to reduce image size at each scale
    // - Min neighbors (1-10): How many neighbors each candidate rectangle should have
    // - Min size (20-100): Minimum possible object size
```

### How it Works
- Uses OpenCV's Haar Cascade Classifier
- Converts image to grayscale for processing
- Detects faces using the cascade classifier
- Draws rectangles around detected faces
- Accessible through menu option 7 in the CLI
- Uses `haarcascade_frontalface_default.xml` from models directory
- Parameters are validated and configurable through the menu

## 4. GUI/CLI Implementation
The command-line interface provides a user-friendly way to interact with all image processing features.

### Implementation Details
```cpp
// src/cli/menu_interface.hpp/cpp
class MenuInterface {
    // Menu-based command-line interface
    // Features:
    // - Interactive menu system
    // - Parameter validation
    // - Error handling
    // - Progress feedback
```

### How it Works
- Provides a user-friendly menu interface
- Shows available operations with descriptions
- Guides users through parameter input
- Validates all inputs before processing
- Provides feedback during and after operations
- Handles errors gracefully

### Menu Options
1. Dilate Image
2. Erode Image
3. Resize Image
4. Adjust Brightness
5. Canny Edge Detection
6. Stitch Images
7. Face Detection
h. Show Help
q. Quit

## Key Implementation Details

### 1. OOP Structure
- All processors inherit from base classes
- Clear separation of concerns
- Encapsulated functionality
- Polymorphic interface

### 2. Error Handling
- Input validation
- File existence checks
- Parameter range validation
- Exception handling

### 3. User Experience
- Clear menu structure
- Help system
- Progress feedback
- Error messages

### 4. File Organization
```
src/
├── core/           # Basic image processing
│   ├── resize_processor.hpp/cpp
│   └── brightness_processor.hpp/cpp
├── advanced/       # Advanced features
│   └── face_detection.hpp/cpp
└── cli/           # User interface
    └── menu_interface.hpp/cpp
```

### 5. Dependencies
- OpenCV for image processing
- C++17 for modern features
- CMake for build system

## Conclusion
This implementation provides a solid foundation for image processing with a user-friendly interface, making it easy to use while maintaining code quality and extensibility. The modular design allows for easy addition of new features and modifications to existing ones. 