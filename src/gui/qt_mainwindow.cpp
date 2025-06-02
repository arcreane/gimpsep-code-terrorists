#include "qt_mainwindow.hpp"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "../core/brightness_processor.hpp"
#include "../core/morphology_processor.hpp"
#include "../core/resize_processor.hpp"
#include "../core/canny_processor.hpp"
#include "../advanced/object_detection.hpp"
#include "../advanced/face_detection.hpp"

namespace ai_slop {

QtMainWindow::QtMainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    inputImageLabel_ = new QLabel("Input Image");
    outputImageLabel_ = new QLabel("Output Image");
    inputImageLabel_->setFixedSize(400, 400);
    outputImageLabel_->setFixedSize(400, 400);
    inputImageLabel_->setStyleSheet("border: 1px solid black");
    outputImageLabel_->setStyleSheet("border: 1px solid black");

    selectImageButton_ = new QPushButton("Select Image");
    featureComboBox_ = new QComboBox();
    featureComboBox_->addItem("Object Detection");
    featureComboBox_->addItem("Face Detection");
    featureComboBox_->addItem("Brightness");
    featureComboBox_->addItem("Resize");
    featureComboBox_->addItem("Morphology");
    featureComboBox_->addItem("Canny Edge");

    paramSlider_ = new QSlider(Qt::Horizontal);
    paramSlider_->setRange(0, 100);
    paramLineEdit_ = new QLineEdit();
    submitButton_ = new QPushButton("Submit");

    QHBoxLayout* imagesLayout = new QHBoxLayout();
    imagesLayout->addWidget(inputImageLabel_);
    imagesLayout->addWidget(outputImageLabel_);

    QVBoxLayout* controlsLayout = new QVBoxLayout();
    controlsLayout->addWidget(selectImageButton_);
    controlsLayout->addWidget(featureComboBox_);
    controlsLayout->addWidget(paramSlider_);
    controlsLayout->addWidget(paramLineEdit_);
    controlsLayout->addWidget(submitButton_);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->addLayout(imagesLayout);
    mainLayout->addLayout(controlsLayout);

    connect(selectImageButton_, &QPushButton::clicked, this, &QtMainWindow::onSelectImage);
    connect(featureComboBox_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QtMainWindow::onFeatureChanged);
    connect(submitButton_, &QPushButton::clicked, this, &QtMainWindow::onSubmit);
}

void QtMainWindow::onSelectImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Select Image", "", "Images (*.png *.jpg *.jpeg)");
    if (!fileName.isEmpty()) {
        inputImage_ = cv::imread(fileName.toStdString());
        if (inputImage_.empty()) {
            QMessageBox::warning(this, "Error", "Failed to load image.");
            return;
        }
        updateImages();
    }
}

void QtMainWindow::onFeatureChanged(int index) {
    // Adjust parameter widgets as needed for each feature
    // For now, just show/hide slider/lineedit
    paramSlider_->setVisible(index == 2 || index == 3); // Brightness/Resize
    paramLineEdit_->setVisible(index == 4); // Morphology
}

void QtMainWindow::onSubmit() {
    processImage();
    updateImages();
}

void QtMainWindow::processImage() {
    if (inputImage_.empty()) return;

    int featureIndex = featureComboBox_->currentIndex();
    double paramValue = paramSlider_->value() / 100.0;  // Convert to 0-1 range
    QString paramText = paramLineEdit_->text();

    switch (featureIndex) {
        case 0: { // Object Detection
            ObjectDetector detector("models/yolo/yolov4-tiny.cfg", 
                                  "models/yolo/yolov4-tiny.weights", 
                                  "models/yolo/coco.names");
            outputImage_ = detector.process(inputImage_);
            break;
        }
        case 1: { // Face Detection
            FaceDetector detector("models/haarcascades/haarcascade_frontalface_default.xml");
            outputImage_ = detector.process(inputImage_);
            break;
        }
        case 2: { // Brightness
            BrightnessProcessor processor(paramValue * 200 - 100);  // Convert to -100 to 100 range
            outputImage_ = processor.process(inputImage_);
            break;
        }
        case 3: { // Resize
            double scale = paramValue * 2.0;  // Convert to 0-2 range
            ResizeProcessor processor(scale);
            outputImage_ = processor.process(inputImage_);
            break;
        }
        case 4: { // Morphology
            // Convert text input to morphology type and kernel size
            MorphologyProcessor::Type type = MorphologyProcessor::Dilate;
            int kernelSize = 3;  // Default to 3x3 kernel
            if (paramText.contains("erode", Qt::CaseInsensitive)) {
                type = MorphologyProcessor::Erode;
            }
            // Try to extract kernel size from text
            bool ok;
            int size = paramText.toInt(&ok);
            if (ok && size > 0) {
                // Ensure kernel size is odd
                kernelSize = size | 1;  // Make odd by setting least significant bit
            }
            MorphologyProcessor processor(type, kernelSize);
            outputImage_ = processor.process(inputImage_);
            break;
        }
        case 5: { // Canny Edge
            double threshold1 = paramValue * 255;  // Convert to 0-255 range
            double threshold2 = threshold1 * 2;    // Second threshold is typically double the first
            CannyProcessor processor(threshold1, threshold2);
            outputImage_ = processor.process(inputImage_);
            break;
        }
        default:
            outputImage_ = inputImage_.clone();
            break;
    }
}

void QtMainWindow::updateImages() {
    if (!inputImage_.empty())
        inputImageLabel_->setPixmap(QPixmap::fromImage(cvMatToQImage(inputImage_)).scaled(inputImageLabel_->size(), Qt::KeepAspectRatio));
    if (!outputImage_.empty())
        outputImageLabel_->setPixmap(QPixmap::fromImage(cvMatToQImage(outputImage_)).scaled(outputImageLabel_->size(), Qt::KeepAspectRatio));
}

QImage QtMainWindow::cvMatToQImage(const cv::Mat& mat) {
    if (mat.type() == CV_8UC3) {
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888);
        return img.copy();
    } else if (mat.type() == CV_8UC1) {
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return img.copy();
    }
    return QImage();
}

} // namespace ai_slop 