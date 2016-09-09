#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->canvas = new Canvas();
    ui->mainGridLayout->addWidget(canvas, 1, 0, 1, 5);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->canvas->n = arg1;
    this->canvas->update();
}
