#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include <QMutex>
#include <QPixmap>

#include "opencv2/opencv.hpp"

class ImageProc
{
private:
    static ImageProc* pInst;
public:
    ImageProc();
    static ImageProc* Get();

    cv::Mat imageProc_toGray(cv::Mat src);
    cv::Mat imageProc_threshold(cv::Mat src);
    cv::Mat imageProc_canny(cv::Mat src, int threshold1, int threshold2);
};

#endif // IMAGEPROC_H
