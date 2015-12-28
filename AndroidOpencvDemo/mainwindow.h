#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QtAndroidExtras>

#include "opencv2/opencv.hpp"

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

public slots:
    void returnImagePath(QString path);

private:
    Ui::MainWindow *ui;

public:
    cv::Mat src;

    static QImage QImageFromMat(cv::Mat mat);
};

#endif // MAINWINDOW_H
