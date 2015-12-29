#include "cameracap.h"

#include <QMutex>
#include <QDebug>

using namespace cv;

CameraCap::CameraCap()
{

}

CameraCap* CameraCap::pInst = NULL;
static QMutex mutex;

CameraCap *CameraCap::Get()
{
    if(pInst == NULL)
    {
        mutex.lock();
        if(pInst == NULL)
            pInst = new CameraCap();
        mutex.unlock();
    }
    return pInst;
}

void CameraCap::run()
{
    VideoCapture cap(0);
    if(!cap.isOpened())
    {
        qDebug() << "Can not open capture";
        return;
    }
    while(1)
    {
        Mat frame;
        cap >> frame;
        emit sendFrame(frame);
    }
}

