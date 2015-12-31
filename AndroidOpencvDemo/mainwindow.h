#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QLabel>
#include <QtAndroidExtras>
#include <QVideoWidget>

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

    void on_pushButton_proc_clicked();

public slots:
    void returnImagePath(QString path);
    void returnFrame(cv::Mat frame);

private:
    Ui::MainWindow *ui;
    QVideoWidget* pVideoWidget;

public:
    cv::Mat src;
    static QImage QImageFromMat(cv::Mat mat);
    void InitDialog();
    void show_image(cv::Mat mat, QLabel *label);
};

#endif // MAINWINDOW_H
