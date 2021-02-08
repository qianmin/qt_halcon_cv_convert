#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"omp.h"
#include<QTime>
#include<QDebug>
#include<QImage>
using namespace std;
using namespace HalconCpp;
using namespace cv;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /////process_here/////


    QTime timer1,timer2,timer3,timer4,timer5;
    //cv_to_h
//    timer1.start();
//    cv::Mat img1=imread("c:/projects/halcon/quexian/1.jpg",-1);
//    HalconCpp::HImage himg1;
//    himg1=cvimg_to_himg(img1);
//    qDebug()<<timer1.elapsed()/1000.0<<"s";


//    //h_to_cv
//    timer2.start();
//    Mat img2=himg_to_cvimg(himg1);
//    qDebug()<<timer2.elapsed()/1000.0<<"s";
//    imshow("test",img2);
//    waitKey(0);

    //h_to_q
//    timer3.start();
//    QImage qimg;
//    if(himg_to_qimg(himg1,qimg))
//    {
//        qDebug()<<timer3.elapsed()/1000.0<<"s";
//        ui->label->setPixmap(QPixmap::fromImage(qimg));
//    }
    //opencv to qimg
    timer4.start();
    Mat cvimg=imread("c:/projects/halcon/quexian/1.jpg",-1);
    QImage qimg=cvimg_to_qimg(cvimg);
    qDebug()<<timer4.elapsed()/1000.0<<"s";
    ui->label->setPixmap(QPixmap::fromImage(qimg));


    //qimg to opencv
    timer5.start();
    Mat cvimg2=qimg_to_cvimg(qimg);
    qDebug()<<timer5.elapsed()/1000.0<<"s";
    cv::imshow("test",cvimg2);
    cv::waitKey(0);


    /////process_here/////

}

MainWindow::~MainWindow()
{
    delete ui;
}



///////////////////API///////////////////////////
Mat MainWindow::himg_to_cvimg(HObject &Hobj)
{
    //get_grayval(Image : : Row, Column : Grayval)

    cv::Mat pImage;
    HTuple htChannels;
    HTuple     width, height;
    width = height = 0;
    //转换图像格式
    ConvertImageType(Hobj, &Hobj, "byte");
    CountChannels(Hobj, &htChannels);
    HTuple cType;
    HTuple grayVal;

    if (htChannels.I() == 1)
    {
        GetImageSize(Hobj, &width, &height);

        pImage = cv::Mat(height, width, CV_8UC1);
        pImage = Mat::zeros(height, width, CV_8UC1);

        for (int i = 0; i < height.I(); ++i)
        {
            for (int j = 0; j < width.I(); ++j)
            {
                GetGrayval(Hobj, i, j, &grayVal);
                pImage.at<uchar>(i, j) = (uchar)grayVal.I();
            }

        }

    }
    else if (htChannels.I() == 3)
    {
        GetImageSize(Hobj, &width, &height);
        pImage = cv::Mat(height, width, CV_8UC3);
        for (int row = 0; row < height.I(); row++)
        {
            for (int col = 0; col < width.I(); col++)
            {
                GetGrayval(Hobj, row, col, &grayVal);

                pImage.at<uchar>(row, col * 3) = (uchar)grayVal[2].I();
                pImage.at<uchar>(row, col * 3 + 1) = (uchar)grayVal[1].I();
                pImage.at<uchar>(row, col * 3 + 2) = (uchar)grayVal[0].I();

            }
        }

    }

    return pImage;
}
HObject MainWindow::cvimg_to_himg(cv::Mat& pImage)
{
    HObject Hobj;
    if (3 == pImage.channels())
    {
        cv::Mat pImageRed, pImageGreen, pImageBlue;
        std::vector<cv::Mat> sbgr(3);
        cv::split(pImage, sbgr);

        int length = pImage.rows * pImage.cols;
        uchar *dataBlue = new uchar[length];
        uchar *dataGreen = new uchar[length];
        uchar *dataRed = new uchar[length];

        int height = pImage.rows;
        int width = pImage.cols;


        for (int row = 0; row < height; row++)
        {
            uchar* ptr = pImage.ptr<uchar>(row);
            for (int col = 0; col < width; col++)
            {
                dataBlue[row * width + col] = ptr[3 * col];
                dataGreen[row * width + col] = ptr[3 * col + 1];
                dataRed[row * width + col] = ptr[3 * col + 2];
            }
        }

        GenImage3(&Hobj, "byte", width, height, (Hlong)(dataRed), (Hlong)(dataGreen), (Hlong)(dataBlue));
        delete[] dataRed;
        delete[] dataGreen;
        delete[] dataBlue;
    }
    else if (1 == pImage.channels())
    {
        int height = pImage.rows;
        int width = pImage.cols;
        uchar *dataGray = new uchar[width * height];
        memcpy(dataGray, pImage.data, width * height);
        GenImage1(&Hobj, "byte", width, height, (Hlong)(dataGray));
        delete[] dataGray;
    }

    return Hobj;
}

bool MainWindow::himg_to_qimg(HalconCpp::HImage &from, QImage &to)
{
    Hlong width;
    Hlong height;
    from.GetImageSize(&width, &height);

    HTuple channels = from.CountChannels();
    HTuple type = from.GetImageType();

    if( strcmp(type[0].S(), "byte" )) // 如果不是 byte 类型，则失败
    {
        return false;
    }

    QImage::Format format;
    switch(channels[0].I())
    {
    case 1:
        format = QImage::Format_Grayscale8;
        break;
    case 3:
        format = QImage::Format_RGB32;
        break;
    default:
        return false;
    }

    if(to.width() != width || to.height() != height || to.format() != format)
    {
        to = QImage(static_cast<int>(width),
                    static_cast<int>(height),
                    format);
    }
    HString Type;
    if(channels[0].I() == 1)
    {
        unsigned char * pSrc = reinterpret_cast<unsigned char *>( from.GetImagePointer1(&Type, &width, &height) );
        memcpy( to.bits(), pSrc, static_cast<size_t>(width) * static_cast<size_t>(height) );
        return true;
    }
    else if(channels[0].I() == 3)
    {
        uchar *R, *G, *B;
        from.GetImagePointer3(reinterpret_cast<void **>(&R),
                              reinterpret_cast<void **>(&G),
                              reinterpret_cast<void **>(&B), &Type, &width, &height);

        for(int row = 0; row < height; row ++)
        {
            QRgb* line = reinterpret_cast<QRgb*>(to.scanLine(row));
            for(int col = 0; col < width; col ++)
            {
                line[col] = qRgb(*R++, *G++, *B++);
            }
        }
        return true;
    }

    return false;
}

QImage MainWindow::cvimg_to_qimg(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
cv::Mat MainWindow::qimg_to_cvimg(QImage image)
{
    cv::Mat mat;
    qDebug() << image.format();
    switch(image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}
