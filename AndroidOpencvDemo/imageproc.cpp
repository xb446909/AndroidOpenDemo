#include "imageproc.h"

using namespace cv;

ImageProc::ImageProc()
{

}

ImageProc* ImageProc::pInst = NULL;

static QMutex mutex;

ImageProc* ImageProc::Get()
{
    if(pInst == NULL)
    {
        mutex.lock();
        if(pInst == NULL)
            pInst = new ImageProc();
        mutex.unlock();
    }
    return pInst;
}


Mat ImageProc::imageProc_toGray(Mat src)
{
    Mat dst;
    if(src.channels() == 4)
        cvtColor(src, dst, CV_BGRA2GRAY);
    else if(src.channels() == 1)
        dst = src.clone();
    else
        cvtColor(src, dst, CV_BGR2GRAY);
    return dst;
}

Mat ImageProc::imageProc_threshold(Mat src)
{
    Mat dst;
    dst = imageProc_toGray(src);
    threshold(dst, dst, 0, 255, THRESH_BINARY  | THRESH_OTSU);
    return dst;
}

Mat ImageProc::imageProc_canny(Mat src, int threshold1, int threshold2)
{
    Mat dst;
    dst = imageProc_toGray(src);
    Canny(dst, dst, threshold1, threshold2);
    return dst;
}

