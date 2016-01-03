#ifndef CAMERACAP_H
#define CAMERACAP_H

#include "opencv2/opencv.hpp"

#include <QObject>
#include <QThread>
#include <QCamera>
#include <QCameraViewfinder>

class CameraCap : public QThread
{
    Q_OBJECT
private:
    static CameraCap* pInst;
public:
    CameraCap();
    static CameraCap* Get();
    void run();
    void setVideoWidget(QCameraViewfinder *pWidget);
    QCamera* pcamera;
signals:
    void sendFrame(cv::Mat frame);
private slots:
    void CameraError(QCamera::Error value);
    void CameraStateChanged(QCamera::State value);
    void CameraStatusChanged(QCamera::Status value);
};

#endif // CAMERACAP_H
