#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "androidimagepicker.h"

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
    QMessageBox::information(NULL, "The path is:", path);

    //ui->label_src->setPixmap(QPixmap::fromImage(*img));
}
