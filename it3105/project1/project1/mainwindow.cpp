#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->board = new Board();
    this->BTAlg = new BackTracking(this->board);

    ui->setupUi(this);
    ui->btRadioButton->click();

    ui->mainGridLayout->addWidget(board, 2, 0, 1, 5);

    ui->queensLineEdit->setValidator( new QIntValidator(this) );



}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_spinBox_valueChanged(int arg1)
{
    this->board->boardSize = arg1;
    this->board->update();
}

void MainWindow::on_solveButton_clicked()
{
    if (ui->btRadioButton->isChecked())
    {
        if (BTAlg->solutions.size() > 0)
        {
            board->queenYPos = BTAlg->solutions[0];
            board->update();
            return;
        }
        BTAlg->solve(board->queenYPos);

        if (BTAlg->solutions.size() > 0)
        {
            board->queenYPos = BTAlg->solutions[0];
            board->update();
        }
    }

}

void MainWindow::on_queensLineEdit_textChanged(const QString &arg1)
{
    board->queenYPos.clear();

    BTAlg->path.clear();
    currentTry = 0;

    for (auto c : arg1)
    {
        int digit = c.toAscii() - '0';
        std::cout << digit << std::endl;
        board->queenYPos.push_back(digit);
    }
    board->update();
}

void MainWindow::on_nextStepButton_clicked()
{
    if (ui->btRadioButton->isChecked())
    {
        if (BTAlg->states.size() == 0)
        {
            BTAlg->solutions.clear();
            BTAlg->solve(board->queenYPos);
        }
        if (BTAlg->states.size() == BTAlg->currentStep)
        {
            BTAlg->currentStep = 0;
            return;
        }

        board->queenYPos = BTAlg->states[BTAlg->currentStep++];

        board->update();
    }
}







