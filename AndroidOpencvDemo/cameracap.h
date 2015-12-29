#ifndef CAMERACAP_H
#define CAMERACAP_H

#include "opencv2/opencv.hpp"

#include <QObject>
#include <QThread>

class CameraCap : public QThread
{
    Q_OBJECT
private:
    static CameraCap* pInst;
public:
    CameraCap();
    static CameraCap* Get();
    void run();
signals:
    void sendFrame(cv::Mat frame);
};

#endif // CAMERACAP_H
