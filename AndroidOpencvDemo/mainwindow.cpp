#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "androidimagepicker.h"

using namespace std;
using namespace cv;

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCamera_triggered()
{
    QMessageBox::information(NULL, "Information", "Open Camera");
}

void MainWindow::on_actionGallery_triggered()
{
    connect(AndroidImagePicker::Get(), SIGNAL(imagePathSignal(QString)), this, SLOT(returnImagePath(QString)));
    AndroidImagePicker::Get()->DoPicker();
}


void MainWindow::returnImagePath(QString path)
{
    src = imread(path.toStdString());
    if(src.data == NULL)
    {
        qDebug() << "read image error";
        return;
    }

    ui->label_src->setPixmap(QPixmap::fromImage(QImageFromMat(src)));
}

QImage MainWindow::QImageFromMat(cv::Mat mat)
{
    QImage img;
    switch(mat.channels())
    {
    case 3:
        cvtColor(mat, mat, CV_BGR2RGB);
        img = QImage(mat.data, mat.cols, mat.rows, QImage::Format_RGB888);
        break;
    case 1:
    case 4:
        img = QImage(mat.data, mat.cols, mat.rows, QImage::Format_ARGB32);
    default:
        break;
    }
    return img;
}
