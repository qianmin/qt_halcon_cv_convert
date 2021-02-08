#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <HalconCpp.h>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace HalconCpp;
using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
//    halcon_opencv
    Mat himg_to_cvimg(HObject &Hobj);
    HObject cvimg_to_himg(cv::Mat& pImage);

//    halcon_to_qimg
    bool himg_to_qimg(HalconCpp::HImage &from, QImage &to);

//  opencv_qimg
    QImage cvimg_to_qimg(const cv::Mat& mat);
    Mat qimg_to_cvimg(QImage image);
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
