#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "androidimagepicker.h"
#include "imageproc.h"
#include "cameracap.h"

#include <QMessageBox>

using namespace std;
using namespace cv;

#define show_src(x) show_image((x), ui->label_src)
#define show_dst(x) show_image((x), ui->label_dst)

static Mat mat_show;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitDialog();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionCamera_triggered()
{
    AndroidImagePicker::Get()->DoPicker(FROM_CAMERA);
}

void MainWindow::on_actionGallery_triggered()
{
    connect(AndroidImagePicker::Get(), SIGNAL(imagePathSignal(QString)), this, SLOT(returnImagePath(QString)));
    AndroidImagePicker::Get()->DoPicker(FROM_GALLERY);
}

void MainWindow::InitDialog()
{
    ui->comboBox_method->insertItem(0, "To Gray");
    ui->comboBox_method->insertItem(0, "Threshold");
    ui->comboBox_method->insertItem(0, "Canny");
}

void MainWindow::returnImagePath(QString path)
{
    qDebug() << "return path: " << path;
    src = imread(path.toStdString());
    if(src.data == NULL)
    {
        qDebug() << "read image error";
        return;
    }
    qDebug() << "show";
    show_src(src);
}

void MainWindow::returnFrame(Mat frame)
{
    show_src(frame);
}

QImage MainWindow::QImageFromMat(cv::Mat mat)
{
    QImage img;
    mat_show = mat.clone();
    switch(mat.channels())
    {
    case 1:
        cvtColor(mat_show, mat_show, CV_GRAY2RGB);
        img = QImage(mat_show.data, mat_show.cols, mat_show.rows, QImage::Format_RGB888);
        break;
    case 3:
        cvtColor(mat_show, mat_show, CV_BGR2RGB);
        img = QImage(mat_show.data, mat_show.cols, mat_show.rows, QImage::Format_RGB888);
        break;
    case 4:
        img = QImage(mat_show.data, mat_show.cols, mat.rows, QImage::Format_ARGB32);
        break;
    default:
        qDebug() << "Undefined channel number";
        break;
    }
    return img;
}

void MainWindow::show_image(cv::Mat mat, QLabel* label)
{
    label->setPixmap(QPixmap::fromImage(QImageFromMat(mat)).scaled(ui->centralWidget->width() * 0.9,
                                                                   label->height(),
                                                                   Qt::KeepAspectRatio));
}

void MainWindow::on_pushButton_proc_clicked()
{
    Mat dst;
    if(ui->comboBox_method->currentText().compare("To Gray") == 0)
    {
        dst = ImageProc::Get()->imageProc_toGray(src);
        show_dst(dst);
    }
    if(ui->comboBox_method->currentText().compare("Threshold") == 0)
    {
        dst = ImageProc::Get()->imageProc_threshold(src);
        show_dst(dst);
    }
    if(ui->comboBox_method->currentText().compare("Canny") == 0)
    {
        dst = ImageProc::Get()->imageProc_canny(src, 0, 30);
        show_dst(dst);
    }
}
