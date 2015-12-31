#ifndef CAMERACAP_H
#define CAMERACAP_H

#include "opencv2/opencv.hpp"

#include <QObject>
#include <QThread>
#include <QCamera>
#include <QVideoWidget>

class CameraCap : public QThread
{
    Q_OBJECT
private:
    static CameraCap* pInst;
    QCamera* pcamera;
public:
    CameraCap();
    static CameraCap* Get();
    void run();
    void setVideoWiget(QVideoWidget* pWidget);
signals:
    void sendFrame(cv::Mat frame);
private slots:
    void CameraError(QCamera::Error value);
    void CameraStateChanged(QCamera::State value);
    void CameraStatusChanged(QCamera::Status value);
};

#endif // CAMERACAP_H
