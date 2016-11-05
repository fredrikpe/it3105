/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
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
    QVBoxLayout *verticalLayout;
    QHBoxLayout *mainLayout;
    QVBoxLayout *verticalLayout_2;
    QPushButton *openFileButton;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QRadioButton *staticButton;
    QRadioButton *linearButton;
    QRadioButton *exponentialButton;
    QPushButton *oneStepButton;
    QHBoxLayout *horizontalLayout;
    QPushButton *nEpochsButton;
    QSpinBox *nEpochsSpinBox;
    QPushButton *solveButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(593, 495);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        mainLayout = new QHBoxLayout();
        mainLayout->setSpacing(6);
        mainLayout->setObjectName(QString::fromUtf8("mainLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        openFileButton = new QPushButton(centralWidget);
        openFileButton->setObjectName(QString::fromUtf8("openFileButton"));

        verticalLayout_2->addWidget(openFileButton);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        staticButton = new QRadioButton(groupBox);
        staticButton->setObjectName(QString::fromUtf8("staticButton"));

        verticalLayout_3->addWidget(staticButton);

        linearButton = new QRadioButton(groupBox);
        linearButton->setObjectName(QString::fromUtf8("linearButton"));

        verticalLayout_3->addWidget(linearButton);

        exponentialButton = new QRadioButton(groupBox);
        exponentialButton->setObjectName(QString::fromUtf8("exponentialButton"));

        verticalLayout_3->addWidget(exponentialButton);


        verticalLayout_2->addWidget(groupBox);

        oneStepButton = new QPushButton(centralWidget);
        oneStepButton->setObjectName(QString::fromUtf8("oneStepButton"));

        verticalLayout_2->addWidget(oneStepButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        nEpochsButton = new QPushButton(centralWidget);
        nEpochsButton->setObjectName(QString::fromUtf8("nEpochsButton"));

        horizontalLayout->addWidget(nEpochsButton);

        nEpochsSpinBox = new QSpinBox(centralWidget);
        nEpochsSpinBox->setObjectName(QString::fromUtf8("nEpochsSpinBox"));
        nEpochsSpinBox->setMinimum(1);
        nEpochsSpinBox->setMaximum(100);
        nEpochsSpinBox->setSingleStep(1);
        nEpochsSpinBox->setValue(1);

        horizontalLayout->addWidget(nEpochsSpinBox);


        verticalLayout_2->addLayout(horizontalLayout);

        solveButton = new QPushButton(centralWidget);
        solveButton->setObjectName(QString::fromUtf8("solveButton"));

        verticalLayout_2->addWidget(solveButton);


        mainLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(mainLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 593, 25));
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
        openFileButton->setText(QApplication::translate("MainWindow", "Open file", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Decay type", 0, QApplication::UnicodeUTF8));
        staticButton->setText(QApplication::translate("MainWindow", "Static", 0, QApplication::UnicodeUTF8));
        linearButton->setText(QApplication::translate("MainWindow", "Linear", 0, QApplication::UnicodeUTF8));
        exponentialButton->setText(QApplication::translate("MainWindow", "Exponential", 0, QApplication::UnicodeUTF8));
        oneStepButton->setText(QApplication::translate("MainWindow", "1 step", 0, QApplication::UnicodeUTF8));
        nEpochsButton->setText(QApplication::translate("MainWindow", "N epochs", 0, QApplication::UnicodeUTF8));
        solveButton->setText(QApplication::translate("MainWindow", "Solve", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
