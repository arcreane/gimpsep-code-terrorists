# AI_SLOP OOP Structure

## 1. Base Classes

### 1.1 ImageProcessor (Abstract Base Class)
```cpp
class ImageProcessor {
public:
    virtual ~ImageProcessor() = default;
    virtual cv::Mat process(const cv::Mat& input) = 0;
    virtual std::string getName() const = 0;

protected:
    void validateInput(const cv::Mat& input) const;
    void validateChannels(const cv::Mat& input, int expected_channels) const;
};
```
- Serves as the base class for all basic image processing operations
- Defines the interface that all processors must implement
- Provides common validation methods
- Uses pure virtual functions to enforce implementation

### 1.2 AdvancedProcessor (Abstract Base Class)
```cpp
class AdvancedProcessor {
public:
    virtual ~AdvancedProcessor() = default;
    virtual cv::Mat process(const cv::Mat& input) = 0;
    virtual std::string getName() const = 0;

protected:
    void validateInput(const cv::Mat& input) const;
};
```
- Base class for more complex processing operations
- Similar interface to ImageProcessor but for advanced features
- Used for operations that require more complex setup or state

## 2. Core Processors (Inherit from ImageProcessor)

### 2.1 BrightnessProcessor
```cpp
class BrightnessProcessor : public ImageProcessor {
public:
    BrightnessProcessor(int brightness);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    int brightness_;
};
```
- Handles brightness adjustment
- Takes brightness value in constructor
- Implements basic image processing

### 2.2 MorphologyProcessor
```cpp
class MorphologyProcessor : public ImageProcessor {
public:
    enum Type { Dilate, Erode };
    MorphologyProcessor(Type type, int kernel_size);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    Type type_;
    int kernel_size_;
};
```
- Handles morphological operations
- Uses enum for operation type
- Configurable kernel size

### 2.3 ResizeProcessor
```cpp
class ResizeProcessor : public ImageProcessor {
public:
    ResizeProcessor(double scale_factor);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    double scale_factor_;
};
```
- Handles image resizing
- Configurable scale factor
- Maintains aspect ratio

### 2.4 CannyProcessor
```cpp
class CannyProcessor : public ImageProcessor {
public:
    CannyProcessor(double threshold1, double threshold2);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    double threshold1_;
    double threshold2_;
};
```
- Implements Canny edge detection
- Configurable thresholds
- Edge detection algorithm

## 3. Advanced Processors (Inherit from AdvancedProcessor)

### 3.1 FaceDetector
```cpp
class FaceDetector : public AdvancedProcessor {
public:
    FaceDetector(const std::string& cascade_file_path,
                 double scale_factor = 1.1,
                 int min_neighbors = 3,
                 cv::Size min_size = cv::Size(30, 30));
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    cv::CascadeClassifier classifier_;
    double scale_factor_;
    int min_neighbors_;
    cv::Size min_size_;
};
```
- Implements face detection
- Uses Haar cascade classifier
- Configurable detection parameters

### 3.2 ObjectDetector
```cpp
class ObjectDetector : public AdvancedProcessor {
public:
    ObjectDetector(const std::string& config_path,
                  const std::string& weights_path,
                  const std::string& names_path,
                  float confidence_threshold = 0.5f,
                  float nms_threshold = 0.4f);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    cv::dnn::Net net_;
    std::vector<std::string> class_names_;
    float confidence_threshold_;
    float nms_threshold_;
};
```
- Implements object detection using YOLO
- Deep learning-based detection
- Configurable detection parameters

### 3.3 VideoProcessor
```cpp
class VideoProcessor : public AdvancedProcessor {
public:
    VideoProcessor(const std::string& input_path, 
                  const std::string& output_path);
    bool process(AdvancedProcessor& processor);
    cv::Mat process(const cv::Mat& input) override;
    std::string getName() const override;
private:
    std::string input_path_;
    std::string output_path_;
    cv::VideoCapture cap_;
    cv::VideoWriter writer_;
};
```
- Handles video processing
- Can apply any processor to video frames
- Manages video I/O

## 4. OOP Design Patterns Used

### 4.1 Strategy Pattern
- Each processor implements a specific processing strategy
- Processors can be swapped at runtime
- Common interface through base classes

### 4.2 Template Method Pattern
- Base classes define the algorithm structure
- Derived classes implement specific steps
- Common validation in base classes

### 4.3 Factory Pattern (Implicit)
- Processors are created based on user selection
- Configuration through constructors
- Encapsulated creation logic

## 5. Key OOP Principles

### 5.1 Encapsulation
- Private member variables
- Public interfaces
- Protected utility methods

### 5.2 Inheritance
- Hierarchical class structure
- Code reuse through base classes
- Polymorphic behavior

### 5.3 Polymorphism
- Virtual functions
- Runtime binding
- Interface-based programming

### 5.4 Abstraction
- Abstract base classes
- Pure virtual functions
- Clear interfaces

## 6. Benefits of the OOP Structure

### 6.1 Modularity
- Each processor is self-contained
- Easy to maintain and modify
- Clear separation of concerns

### 6.2 Extensibility
- Easy to add new processors
- No modification of existing code needed
- Follows Open-Closed Principle

### 6.3 Maintainability
- Clear class hierarchy
- Consistent interface
- Well-defined responsibilities

### 6.4 Reusability
- Common functionality in base classes
- Shared validation methods
- Consistent error handling 