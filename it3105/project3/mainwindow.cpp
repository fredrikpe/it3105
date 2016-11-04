#include <vector>
#include <memory>
#include <iostream>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    canvas(new Canvas(this))
{
    ui->setupUi(this);



    ui->mainLayout->addWidget(canvas);




    std::vector<pair<double, double>> testData;



    for (int i=1; i<10; i++)
    {
        pair<double, double> wv = {1.0/i, 1.0/(10 - i)};
        testData.push_back(wv);
    }

    som = std::make_shared<SelfOrganizingMap>(testData);
    som->radiusType = STATIC;
    canvas->som = som;


}




MainWindow::~MainWindow()
{
    delete canvas;
}

void MainWindow::on_nextIterationButton_clicked()
{
    std::cout << "City index: " << currentCityIndex << std::endl;
    som->one_iteration(som->cities[currentCityIndex]);
    currentCityIndex = (currentCityIndex + 1) % som->num_of_cities;
    canvas->update();
}
