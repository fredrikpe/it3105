#include <vector>
#include <memory>
#include <iostream>
#include <ctime>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    canvas(new Canvas(this))
{
    ui->setupUi(this);



    ui->mainLayout->addWidget(canvas);
    ui->mainLayout->setStretch(1, 1);


    som = std::make_shared<SelfOrganizingMap>();
    canvas->som = som;

    // Init
    ui->staticButton->click();
    ui->staticInfluenceButton->click();

}

void MainWindow::on_oneStepButton_clicked()
{
    som->one_step(som->cities[currentCityIndex]);
    currentCityIndex = (currentCityIndex + 1) % som->num_of_cities;
    canvas->update();
}

void MainWindow::on_nEpochsButton_clicked()
{
    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i=0; i<n_epochs; i++)
    {
        som->one_epoch();
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    cout << "Time doing " << n_epochs << " epochs: " << elapsed << " s" << endl;
    canvas->update();
}

void MainWindow::on_solveButton_clicked()
{
    som->makeTour();
    canvas->update();
}

void MainWindow::on_nEpochsSpinBox_editingFinished()
{

    n_epochs = ui->nEpochsSpinBox->value();
}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open file"), "../project3", tr("City Files (*.txt)"));

    QFile file(fileName);

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0, "Error opening file.", file.errorString());
    }

    QTextStream in(&file);

    std::vector<pair<double, double>> cities;

    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(" ");

        if (fields[0] == "DIMENSION")
        {
            int dimension = fields[2].toInt();

            in.readLine();
            in.readLine();

            for (int i=0; i<dimension; i++)
            {
                QString line = in.readLine();
                fields = line.split(" ");

                pair<double, double> city = {fields[1].toDouble(), fields[2].toDouble()};
                cities.push_back(city);
            }
        }
    }
    file.close();

    normalize(cities);

    som->newCityInstance(cities);
    som->tour_indexes.clear();

    currentCityIndex = 0;
}

void MainWindow::normalize(vector<pair<double, double>>& cityMap)
{
    double min_x = INFINITY;
    double min_y = INFINITY;
    double max_x = 0;
    double max_y = 0;
    for (auto&& city : cityMap)
    {
        max_x = city.first > max_x ? city.first : max_x;
        min_x = city.first < min_x ? city.first : min_x;
        max_y = city.second > max_y ? city.second : max_y;
        min_y = city.second < min_y ? city.second : min_y;
    }

    double dist_x = max_x - min_x;
    double dist_y = max_y - min_y;

    som->x_scaling = dist_x;
    som->y_scaling = dist_y;

    for (auto&& city : cityMap)
    {
        city.first = (city.first - min_x) / dist_x;
        city.second = (city.second - min_y) / dist_y;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}

void MainWindow::reset()
{
    if (som->cities.size() != 0)
    {
        currentCityIndex = 0;
        som->epoch = 1;
        som->tour_distance = 0;
        som->nodes.clear();
        som->tour_indexes.clear();
        som->generateCircularCityMap(som->nodes, 0.25, som->num_of_nodes);
        canvas->update();
    }
}

void MainWindow::on_staticButton_clicked()
{
    changeType(som->decay_type, STATIC);
}

void MainWindow::on_linearButton_clicked()
{
    changeType(som->decay_type, LINEAR);
}

void MainWindow::on_exponentialButton_clicked()
{
    changeType(som->decay_type, EXPONENTIAL);
}

void MainWindow::changeType(DecayType &old, DecayType nu)
{
    if (old != nu)
    {
        old = nu;
        reset();
    }
}

void MainWindow::on_numIterationsspinBox_editingFinished()
{
    som->num_of_iterations = ui->numIterationsspinBox->value();
    reset();
}

void MainWindow::on_staticInfluenceButton_clicked()
{
    changeType(som->influence_type, STATIC);
}

void MainWindow::on_exponentialInfluenceButton_clicked()
{
    changeType(som->influence_type, EXPONENTIAL);
}
