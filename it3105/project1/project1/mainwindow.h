#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "board.h"
#include "backtracking.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Board *board;
    BackTracking *BTAlg;
    int currentTry = 0;

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_solveButton_clicked();

    void on_queensLineEdit_textChanged(const QString &arg1);

    void on_nextStepButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
