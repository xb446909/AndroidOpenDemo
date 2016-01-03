#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QtAndroidExtras>
#include <QQuickItem>
#include "cameracap.h"
#include "imageproc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionCamera_triggered();

    void on_actionGallery_triggered();

    void on_pushButton_proc_clicked();

public slots:
    void returnImagePath(QString path);

private:
    Ui::MainWindow *ui;
    QObject *cameraObj, *videoOutputObj, *imageCaptureObj;

public:
    cv::Mat src;
    static QImage QImageFromMat(cv::Mat mat);
    void InitDialog();
    void show_image(cv::Mat mat, QLabel *label);
    void doProcess();
    void takeShot();
};

#endif // MAINWINDOW_H
