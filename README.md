# 1 qt_halcon_cv_convert
data convert between halcon,qt,opencv,using C++.
```
//    halcon_opencv
    Mat himg_to_cvimg(HObject &Hobj);
    HObject cvimg_to_himg(cv::Mat& pImage);

//    halcon_to_qimg
    bool himg_to_qimg(HalconCpp::HImage &from, QImage &to);

//  opencv_qimg
    QImage cvimg_to_qimg(const cv::Mat& mat);
    Mat qimg_to_cvimg(QImage image);
```


# 2 a succesefull convert projects
# 3 inviroments
```
halcon20.11
opencv4.5.1
qt5.15.2 MSVC2019

amd2700
gtx1650
32GB 3000 ddr4
```

# 4:results
![cnm](./time.png)
