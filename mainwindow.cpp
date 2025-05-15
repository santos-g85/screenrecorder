#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "screenrecorder.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    recorder = new ScreenRecorder(this);

    connect(ui->startbtn,SIGNAL(released()),this,SLOT(on_startbtn_clicked()));
    connect(ui->stopbtn,SIGNAL(released()),this,SLOT(on_stopbtn_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startbtn_clicked()
{
     recorder->startRecorder();
}


void MainWindow::on_stopbtn_clicked()
{
    recorder->stopRecorder();
}

