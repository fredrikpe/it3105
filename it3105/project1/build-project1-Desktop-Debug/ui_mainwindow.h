/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout_5;
    QGridLayout *mainGridLayout;
    QLabel *label_5;
    QLabel *label;
    QSpinBox *spinBox;
    QLineEdit *lineEdit_5;
    QLabel *label_6;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_6;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_6;
    QVBoxLayout *verticalLayout;
    QRadioButton *btRadioButton;
    QRadioButton *tsRadioButton;
    QRadioButton *saRadioButton;
    QRadioButton *gaRadioButton;
    QLabel *label_7;
    QPushButton *solveButton;
    QLineEdit *queensLineEdit;
    QPushButton *nextStepButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(585, 557);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_5 = new QGridLayout(centralWidget);
        gridLayout_5->setSpacing(6);
        gridLayout_5->setContentsMargins(11, 11, 11, 11);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        mainGridLayout = new QGridLayout();
        mainGridLayout->setSpacing(6);
        mainGridLayout->setObjectName(QString::fromUtf8("mainGridLayout"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        mainGridLayout->addWidget(label_5, 7, 3, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        mainGridLayout->addWidget(label, 0, 0, 1, 1);

        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QString::fromUtf8("spinBox"));
        spinBox->setMinimum(4);
        spinBox->setMaximum(10);

        mainGridLayout->addWidget(spinBox, 0, 1, 1, 1);

        lineEdit_5 = new QLineEdit(centralWidget);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));

        mainGridLayout->addWidget(lineEdit_5, 8, 3, 1, 1);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        mainGridLayout->addWidget(label_6, 7, 4, 1, 1);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        mainGridLayout->addWidget(label_3, 7, 1, 1, 1);

        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));

        mainGridLayout->addWidget(lineEdit_3, 8, 1, 1, 1);

        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        mainGridLayout->addWidget(label_4, 7, 2, 1, 1);

        lineEdit_4 = new QLineEdit(centralWidget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));

        mainGridLayout->addWidget(lineEdit_4, 8, 2, 1, 1);

        lineEdit_6 = new QLineEdit(centralWidget);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));

        mainGridLayout->addWidget(lineEdit_6, 8, 4, 1, 1);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_6 = new QGridLayout(groupBox);
        gridLayout_6->setSpacing(6);
        gridLayout_6->setContentsMargins(11, 11, 11, 11);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        btRadioButton = new QRadioButton(groupBox);
        btRadioButton->setObjectName(QString::fromUtf8("btRadioButton"));

        verticalLayout->addWidget(btRadioButton);

        tsRadioButton = new QRadioButton(groupBox);
        tsRadioButton->setObjectName(QString::fromUtf8("tsRadioButton"));

        verticalLayout->addWidget(tsRadioButton);

        saRadioButton = new QRadioButton(groupBox);
        saRadioButton->setObjectName(QString::fromUtf8("saRadioButton"));

        verticalLayout->addWidget(saRadioButton);

        gaRadioButton = new QRadioButton(groupBox);
        gaRadioButton->setObjectName(QString::fromUtf8("gaRadioButton"));

        verticalLayout->addWidget(gaRadioButton);


        gridLayout_6->addLayout(verticalLayout, 0, 0, 1, 1);


        mainGridLayout->addWidget(groupBox, 0, 5, 7, 1);

        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        mainGridLayout->addWidget(label_7, 1, 0, 1, 1);

        solveButton = new QPushButton(centralWidget);
        solveButton->setObjectName(QString::fromUtf8("solveButton"));

        mainGridLayout->addWidget(solveButton, 1, 4, 1, 1);

        queensLineEdit = new QLineEdit(centralWidget);
        queensLineEdit->setObjectName(QString::fromUtf8("queensLineEdit"));

        mainGridLayout->addWidget(queensLineEdit, 1, 1, 1, 1);

        nextStepButton = new QPushButton(centralWidget);
        nextStepButton->setObjectName(QString::fromUtf8("nextStepButton"));

        mainGridLayout->addWidget(nextStepButton, 1, 3, 1, 1);

        mainGridLayout->setRowStretch(2, 1);

        gridLayout_5->addLayout(mainGridLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 585, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "SA", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Board size", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "GA", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "BT", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "TS", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Algorithm", 0, QApplication::UnicodeUTF8));
        btRadioButton->setText(QApplication::translate("MainWindow", "BT", 0, QApplication::UnicodeUTF8));
        tsRadioButton->setText(QApplication::translate("MainWindow", "TS", 0, QApplication::UnicodeUTF8));
        saRadioButton->setText(QApplication::translate("MainWindow", "SA", 0, QApplication::UnicodeUTF8));
        gaRadioButton->setText(QApplication::translate("MainWindow", "GA", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Queens", 0, QApplication::UnicodeUTF8));
        solveButton->setText(QApplication::translate("MainWindow", "Solve", 0, QApplication::UnicodeUTF8));
        nextStepButton->setText(QApplication::translate("MainWindow", "Next Step", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
