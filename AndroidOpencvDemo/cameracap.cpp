#include "cameracap.h"

#include <QMutex>
#include <QDebug>
#include <QCameraInfo>

using namespace cv;

CameraCap::CameraCap()
{
    pcamera = new QCamera(this);
    connect(pcamera, SIGNAL(error(QCamera::Error)), this, SLOT(CameraError(QCamera::Error)));
    connect(pcamera, SIGNAL(stateChanged(QCamera::State)), this, SLOT(CameraStateChanged(QCamera::State)));
    connect(pcamera, SIGNAL(statusChanged(QCamera::Status)), this, SLOT(CameraStatusChanged(QCamera::Status)));
    pcamera->load();
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
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
        qDebug() << cameraInfo.deviceName();
}

void CameraCap::setVideoWiget(QVideoWidget *pWidget)
{
    pcamera->setViewfinder(pWidget);
}


void CameraCap::CameraError(QCamera::Error value)
{
    switch (value)
    {
    case QCamera::NoError:
        qDebug() << "Camera: No errors have occurred.";
        break;
    case QCamera::CameraError:
        qDebug() << "Camera: An error has occurred.";
        break;
    case QCamera::InvalidRequestError:
        qDebug() << "Camera: System resource doesn't support requested functionality.";
        break;
    case QCamera::ServiceMissingError:
        qDebug() << "Camera: No camera service available.";
        break;
    case QCamera::NotSupportedFeatureError:
        qDebug() << "Camera: The feature is not supported.";
        break;
    default:
        break;
    }
}

void CameraCap::CameraStateChanged(QCamera::State value)
{
    switch (value)
    {
    case QCamera::UnloadedState:
        qDebug() << "Camera: Camera not loaded.";
        break;
    case QCamera::LoadedState:
        qDebug() << "Camera: The camera is loaded and ready to be configured.";
        break;
    case QCamera::ActiveState:
        qDebug() << "Camera: Camera In the active state as soon as camera is started the\
                     viewfinder displays video frames and the camera is ready for capture.";
        break;
    default:
        break;
    }
}

void CameraCap::CameraStatusChanged(QCamera::Status value)
{
    switch (value)
    {
    case QCamera::UnavailableStatus:
        qDebug() << "Camera: The camera or camera backend is not available.";
        break;
    case QCamera::UnloadedStatus:
        qDebug() << "Camera: The initial camera status, with camera not loaded.";
        break;
    case QCamera::UnloadingStatus:
        qDebug() << "Camera: The camera device is unloading.";
        break;
    case QCamera::LoadingStatus:
        qDebug() << "Camera: The camera device loading";
        break;
    case QCamera::LoadedStatus:
        qDebug() << "Camera: The camera is loaded and ready to be configured.";
        break;
    case QCamera::StandbyStatus:
        qDebug() << "Camera: The camera is in the power saving standby mode.";
        break;
    case QCamera::StoppingStatus:
        qDebug() << "Camera: The camera is stopping.";
        break;
    case QCamera::StartingStatus:
        qDebug() << "Camera: The camera is starting.";
        break;
    default:
        break;
    }
}

