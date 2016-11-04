#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <memory>

#include "selforganizingmap.h"
#include "canvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void drawCanvas();

private slots:
    void on_nextIterationButton_clicked();

private:

    Ui::MainWindow *ui;
    Canvas *canvas;

    std::shared_ptr<SelfOrganizingMap> som;
    int currentCityIndex = 0;
};

#endif // MAINWINDOW_H
