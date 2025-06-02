#ifndef AI_SLOP_QT_MAINWINDOW_HPP
#define AI_SLOP_QT_MAINWINDOW_HPP

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QLineEdit>
#include <QComboBox>
#include <QImage>
#include <QPixmap>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <opencv2/opencv.hpp>

namespace ai_slop {

class QtMainWindow : public QMainWindow {
    Q_OBJECT
public:
    QtMainWindow(QWidget* parent = nullptr);

private slots:
    void onSelectImage();
    void onFeatureChanged(int index);
    void onSubmit();

private:
    void updateImages();
    void processImage();
    QImage cvMatToQImage(const cv::Mat& mat);

    QLabel* inputImageLabel_;
    QLabel* outputImageLabel_;
    QPushButton* selectImageButton_;
    QComboBox* featureComboBox_;
    QSlider* paramSlider_;
    QLineEdit* paramLineEdit_;
    QPushButton* submitButton_;

    cv::Mat inputImage_;
    cv::Mat outputImage_;
};

} // namespace ai_slop

#endif // AI_SLOP_QT_MAINWINDOW_HPP 