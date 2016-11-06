#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QStringList>
#include <QFile>

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
    void on_oneStepButton_clicked();

    void on_solveButton_clicked();

    void on_openFileButton_clicked();

    void normalize(vector<pair<double, double>>& cityMap);

    void on_nEpochsButton_clicked();

    void on_nEpochsSpinBox_editingFinished();

    void on_staticButton_clicked();

    void on_linearButton_clicked();

    void on_exponentialButton_clicked();

    void changeType(DecayType &old, DecayType nu);

    void reset();

    void on_numIterationsspinBox_editingFinished();

    void on_staticInfluenceButton_clicked();

    void on_exponentialInfluenceButton_clicked();

private:

    Ui::MainWindow *ui;
    Canvas *canvas;

    std::shared_ptr<SelfOrganizingMap> som;
    int currentCityIndex = 0;
    int n_epochs = 1;
};

#endif // MAINWINDOW_H
