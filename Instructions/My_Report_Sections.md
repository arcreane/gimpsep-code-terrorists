# My Report Sections

## 1. Resize Feature

### Overview
The resize feature in our image processing application provides users with the ability to scale images up or down while maintaining aspect ratio and image quality. This feature is essential for preparing images for different purposes, such as web display, printing, or further processing.

### Technical Implementation
The resize functionality is implemented through the `ResizeProcessor` class, which inherits from the base `ImageProcessor` class. Here's the core implementation:

```cpp
class ResizeProcessor : public ImageProcessor {
public:
    ResizeProcessor() : ImageProcessor("Resize") {}
    
    cv::Mat process(const cv::Mat& input, const std::map<std::string, double>& params) override {
        double factor = params.at("factor");
        if (factor < 0.1 || factor > 10.0) {
            throw std::invalid_argument("Resize factor must be between 0.1 and 10.0");
        }
        
        cv::Mat output;
        cv::resize(input, output, cv::Size(), factor, factor, cv::INTER_LINEAR);
        return output;
    }
};
```

Key implementation details:
- **Parameter Validation**: The resize factor is strictly bounded between 0.1 and 10.0 to prevent extreme scaling
- **Interpolation Method**: Uses `cv::INTER_LINEAR` for optimal balance between quality and performance
- **Aspect Ratio**: Maintained automatically through uniform scaling in both dimensions
- **Memory Management**: Efficient handling of image data through OpenCV's Mat class

### Processing Pipeline
1. **Input Validation**:
   ```cpp
   if (factor < 0.1 || factor > 10.0) {
       throw std::invalid_argument("Resize factor must be between 0.1 and 10.0");
   }
   ```

2. **Image Resizing**:
   ```cpp
   cv::resize(input, output, cv::Size(), factor, factor, cv::INTER_LINEAR);
   ```

### Performance Optimization
- **Memory Efficiency**: Direct manipulation of image data
- **Quality Control**: Linear interpolation for smooth scaling
- **Error Handling**: Comprehensive input validation
- **Resource Management**: Automatic memory management through OpenCV

### User Interface
The resize feature is accessible through the CLI menu system (Option 3). The interface:
- Prompts users for the desired resize factor
- Validates input to ensure it's within the acceptable range
- Provides immediate visual feedback
- Handles errors gracefully with informative messages

### Use Cases
1. **Image Preparation**: Resizing images for specific display requirements
2. **Storage Optimization**: Reducing image size for storage efficiency
3. **Quality Enhancement**: Upscaling images for better detail
4. **Batch Processing**: Preparing multiple images to uniform dimensions

## 2. Brightness Adjustment (Lighten/Darken)

### Overview
The brightness adjustment feature provides users with the ability to modify the luminance of images, allowing for both lightening and darkening effects. This feature is particularly useful for correcting underexposed or overexposed images, enhancing visibility in dark areas, or creating artistic effects.

### Technical Implementation
The brightness adjustment is implemented using OpenCV's efficient image processing capabilities. Here's the core implementation:

```cpp
class BrightnessProcessor : public ImageProcessor {
public:
    BrightnessProcessor() : ImageProcessor("Brightness") {}
    
    cv::Mat process(const cv::Mat& input, const std::map<std::string, double>& params) override {
        double value = params.at("value");
        if (value < -255 || value > 255) {
            throw std::invalid_argument("Brightness value must be between -255 and 255");
        }
        
        cv::Mat output;
        input.convertTo(output, -1, 1, value);
        return output;
    }
};
```

Key implementation details:
- **Parameter Range**: Brightness values from -255 to 255
- **Processing Method**: Uses `convertTo` for efficient pixel-level operations
- **Type Preservation**: Maintains original image type through `-1` parameter
- **Memory Efficiency**: Direct manipulation of image data

### Processing Pipeline
1. **Input Validation**:
   ```cpp
   if (value < -255 || value > 255) {
       throw std::invalid_argument("Brightness value must be between -255 and 255");
   }
   ```

2. **Brightness Adjustment**:
   ```cpp
   input.convertTo(output, -1, 1, value);
   ```

### Performance Optimization
- **Efficient Processing**: Uses OpenCV's optimized `convertTo` function
- **Memory Management**: Direct manipulation of image data
- **Type Safety**: Preserves image type and format
- **Error Handling**: Comprehensive input validation

### User Interface
The brightness adjustment is accessible through the CLI menu system (Option 4). The interface:
- Prompts users for the desired brightness value
- Validates input to ensure it's within the acceptable range
- Provides immediate visual feedback
- Handles errors gracefully with informative messages

### Use Cases
1. **Image Correction**: Adjusting underexposed or overexposed images
2. **Detail Enhancement**: Revealing details in dark or bright areas
3. **Artistic Effects**: Creating high-key or low-key images
4. **Pre-processing**: Preparing images for other processing operations

## 3. Advanced Features

### 3.1 Face Detection
The face detection feature represents an advanced computer vision capability that automatically identifies and locates human faces within images. This feature uses the Haar Cascade Classifier, a machine learning-based approach where a cascade function is trained from many positive and negative images.

### Technical Implementation
The face detection system is implemented through the `FaceDetector` class. Here's the core implementation:

```cpp
class FaceDetector : public AdvancedProcessor {
private:
    cv::CascadeClassifier face_cascade;
    
public:
    FaceDetector() : AdvancedProcessor("Face Detection") {
        if (!face_cascade.load("models/haarcascade_frontalface_default.xml")) {
            throw std::runtime_error("Error loading face cascade classifier");
        }
    }
    
    cv::Mat process(const cv::Mat& input, const std::map<std::string, double>& params) override {
        double scale_factor = params.at("scale_factor");
        int min_neighbors = static_cast<int>(params.at("min_neighbors"));
        int min_size = static_cast<int>(params.at("min_size"));
        
        cv::Mat gray, output = input.clone();
        cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
        
        std::vector<cv::Rect> faces;
        face_cascade.detectMultiScale(gray, faces, scale_factor, min_neighbors, 
                                    0, cv::Size(min_size, min_size));
        
        for (const auto& face : faces) {
            cv::rectangle(output, face, cv::Scalar(0, 255, 0), 2);
        }
        
        return output;
    }
};
```

### Processing Pipeline
1. **Image Preprocessing**:
   ```cpp
   cv::Mat gray;
   cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
   ```

2. **Face Detection**:
   ```cpp
   face_cascade.detectMultiScale(gray, faces, scale_factor, min_neighbors, 
                               0, cv::Size(min_size, min_size));
   ```

3. **Result Visualization**:
   ```cpp
   for (const auto& face : faces) {
       cv::rectangle(output, face, cv::Scalar(0, 255, 0), 2);
   }
   ```

### Performance Considerations
- **Classifier Loading**: Efficient loading of pre-trained model
- **Memory Management**: Proper handling of image data
- **Parameter Optimization**: Configurable detection parameters
- **Error Handling**: Comprehensive validation and error checking

### 3.2 Object Detection
The object detection feature uses the YOLO (You Only Look Once) deep learning model to detect and classify multiple objects within images. This advanced feature provides real-time object detection capabilities with high accuracy.

#### Technical Implementation
```cpp
class ObjectDetector : public AdvancedProcessor {
public:
    ObjectDetector(const std::string& config_path,
                  const std::string& weights_path,
                  const std::string& names_path,
                  float confidence_threshold = 0.5f,
                  float nms_threshold = 0.4f);
    
    cv::Mat process(const cv::Mat& input) override;
};
```

Key features:
- **YOLO Integration**: Uses pre-trained YOLO model for object detection
- **Multiple Classes**: Can detect and classify multiple object types
- **Confidence Scoring**: Configurable confidence thresholds
- **Non-Maximum Suppression**: Eliminates redundant detections

### 3.3 Video Processing
The video processing feature provides advanced capabilities for processing video files, including frame-by-frame analysis and background subtraction.

#### Technical Implementation
```cpp
class VideoProcessor : public AdvancedProcessor {
public:
    VideoProcessor(const std::string& input_path, 
                  const std::string& output_path);
    
    bool process(AdvancedProcessor& processor);
};
```

Key features:
- **Frame Processing**: Process video frames individually
- **Background Subtraction**: MOG2 algorithm for motion detection
- **Grayscale Conversion**: Convert videos to grayscale
- **Real-time Processing**: Efficient frame-by-frame handling

### 3.4 Image Inpainting
The inpainting feature provides content-aware image restoration, allowing for the removal of unwanted elements or the restoration of damaged areas in images.

#### Technical Implementation
```cpp
class Inpainter : public AdvancedProcessor {
public:
    Inpainter(double inpaint_radius = 3.0,
              int method = cv::INPAINT_NS);
    
    cv::Mat process(const cv::Mat& input, const cv::Mat& mask);
};
```

Key features:
- **Content-Aware Restoration**: Intelligent filling of missing areas
- **Multiple Methods**: Support for different inpainting algorithms
- **Configurable Parameters**: Adjustable inpaint radius
- **Mask-Based Processing**: Precise control over areas to inpaint

## 4. Command-Line Interface (CLI)

### Overview
The project implements a user-friendly command-line interface that provides intuitive access to all image processing features. The CLI is designed to be both powerful and accessible, catering to both novice and experienced users.

### Technical Implementation
The CLI is implemented through the `MenuInterface` class. Here's the core implementation:

```cpp
class MenuInterface {
private:
    std::map<int, std::shared_ptr<ImageProcessor>> processors;
    
public:
    void addProcessor(int id, std::shared_ptr<ImageProcessor> processor) {
        processors[id] = processor;
    }
    
    void run() {
        while (true) {
            displayMenu();
            int choice = getChoice();
            
            if (choice == 'q') break;
            if (choice == 'h') {
                displayHelp();
                continue;
            }
            
            auto it = processors.find(choice);
            if (it != processors.end()) {
                processImage(it->second);
            } else {
                std::cout << "Invalid choice. Please try again.\n";
            }
        }
    }
    
private:
    void displayMenu() {
        std::cout << "\nImage Processing Menu:\n";
        std::cout << "1. Dilate Image\n";
        std::cout << "2. Erode Image\n";
        std::cout << "3. Resize Image\n";
        std::cout << "4. Adjust Brightness\n";
        std::cout << "5. Canny Edge Detection\n";
        std::cout << "6. Stitch Images\n";
        std::cout << "7. Face Detection\n";
        std::cout << "h. Show Help\n";
        std::cout << "q. Quit\n";
        std::cout << "Enter your choice: ";
    }
};
```

### Implementation Details
- **Menu System**: Structured menu with clear options
- **Input Validation**: Comprehensive input checking
- **Error Handling**: Graceful error management
- **User Feedback**: Clear progress and status messages

### Benefits
1. **Accessibility**: Easy to use for all skill levels
2. **Efficiency**: Quick access to all features
3. **Reliability**: Robust error handling
4. **Extensibility**: Easy to add new features 