# UML Class Diagram for AI_SLOP Project

## Diagram

```plaintext
+----------------------+
|   AdvancedProcessor  |  <<abstract>>
+----------------------+
| +process(input): cv::Mat (pure virtual) |
| +getName(): std::string (pure virtual)  |
+----------------------+
           ^
           |
+---------------------+      +-------------------+      +-------------------+
|   ObjectDetector    |      |   FaceDetector    |      |  VideoProcessor   |
+---------------------+      +-------------------+      +-------------------+
| -net_: cv::dnn::Net |      | -classifier_:     |      | -input_path_:     |
| -class_names_:      |      |   cv::Cascade...  |      | -output_path_:    |
| -confidence_: float |      | -scale_factor_:   |      | -cap_: VideoCap...|
| -nms_: float        |      | -min_neighbors_:  |      | -writer_: Video...|
| -input_width_: int  |      | -min_size_:       |      | -frame_width_: int|
| -input_height_: int |      +-------------------+      | -frame_height_:int|
+---------------------+      | +process(input):  |      | -fps_: double     |
| +process(input):    |      |   cv::Mat         |      +-------------------+
|   cv::Mat           |      +-------------------+      | +process(input):  |
+---------------------+                                 |   cv::Mat         |
                                                        +-------------------+

+----------------------+
|   ImageProcessor     |  <<abstract>>
+----------------------+
| +process(input): cv::Mat (pure virtual) |
| +getName(): std::string (pure virtual)  |
+----------------------+
           ^
           |
+---------------------+   +---------------------+   +---------------------+   +---------------------+
| BrightnessProcessor |   | ResizeProcessor     |   | MorphologyProcessor |   | CannyProcessor      |
+---------------------+   +---------------------+   +---------------------+   +---------------------+
| -value_: int        |   | -factor_: double    |   | -type_: enum        |   | -threshold1_: double|
+---------------------+   +---------------------+   | -kernel_size_: int  |   | -threshold2_: double|
| +process(input):    |   | +process(input):    |   +---------------------+   +---------------------+
|   cv::Mat           |   |   cv::Mat           |   | +process(input):    |   | +process(input):    |
+---------------------+   +---------------------+   |   cv::Mat           |   |   cv::Mat           |
                                                    +---------------------+   +---------------------+

+----------------------+
|   MenuInterface      |
+----------------------+
| -running: bool       |
| -main_menu_actions:  |
|   map<string, func>  |
| -operation_desc: map |
+----------------------+
| +run()               |
| +executeOperation()  |
| +processImage()      |
| +processVideo()      |
| +collect*Params()    |
+----------------------+
```

## Explanation

- **AdvancedProcessor**: Abstract base class for advanced features (object detection, face detection, video processing). Defines pure virtual methods `process` and `getName`.
- **ObjectDetector**: Detects objects using YOLO. Encapsulates network, class names, and parameters.
- **FaceDetector**: Detects faces using Haar cascades. Encapsulates classifier and parameters.
- **VideoProcessor**: Processes videos (grayscale conversion). Encapsulates input/output paths and video properties.
- **ImageProcessor**: Abstract base class for core image operations (brightness, resize, etc.). Defines pure virtual methods `process` and `getName`.
- **BrightnessProcessor, ResizeProcessor, MorphologyProcessor, CannyProcessor**: Concrete classes for image operations, each encapsulating specific parameters and logic.
- **MenuInterface**: CLI interface. Encapsulates menu logic and user interaction, using processor objects to perform operations.

## Key Relationships
- **Inheritance**: Advanced and core processors inherit from abstract base classes.
- **Composition**: MenuInterface uses processor objects.
- **Encapsulation**: Each class encapsulates its parameters and logic. 