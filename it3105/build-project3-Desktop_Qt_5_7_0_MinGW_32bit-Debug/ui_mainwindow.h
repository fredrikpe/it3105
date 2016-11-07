/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QRadioButton *staticInfluenceButton;
    QRadioButton *exponentialInfluenceButton;
    QLabel *label;
    QSpinBox *numIterationsspinBox;
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(593, 495);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        mainLayout = new QHBoxLayout();
        mainLayout->setSpacing(6);
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        openFileButton = new QPushButton(centralWidget);
        openFileButton->setObjectName(QStringLiteral("openFileButton"));

        verticalLayout_2->addWidget(openFileButton);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        staticButton = new QRadioButton(groupBox);
        staticButton->setObjectName(QStringLiteral("staticButton"));

        verticalLayout_3->addWidget(staticButton);

        linearButton = new QRadioButton(groupBox);
        linearButton->setObjectName(QStringLiteral("linearButton"));

        verticalLayout_3->addWidget(linearButton);

        exponentialButton = new QRadioButton(groupBox);
        exponentialButton->setObjectName(QStringLiteral("exponentialButton"));

        verticalLayout_3->addWidget(exponentialButton);


        verticalLayout_2->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        staticInfluenceButton = new QRadioButton(groupBox_2);
        staticInfluenceButton->setObjectName(QStringLiteral("staticInfluenceButton"));

        verticalLayout_4->addWidget(staticInfluenceButton);

        exponentialInfluenceButton = new QRadioButton(groupBox_2);
        exponentialInfluenceButton->setObjectName(QStringLiteral("exponentialInfluenceButton"));

        verticalLayout_4->addWidget(exponentialInfluenceButton);


        verticalLayout_2->addWidget(groupBox_2);

        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);

        verticalLayout_2->addWidget(label);

        numIterationsspinBox = new QSpinBox(centralWidget);
        numIterationsspinBox->setObjectName(QStringLiteral("numIterationsspinBox"));
        numIterationsspinBox->setMinimum(5);
        numIterationsspinBox->setMaximum(500);
        numIterationsspinBox->setSingleStep(5);
        numIterationsspinBox->setValue(20);

        verticalLayout_2->addWidget(numIterationsspinBox);

        oneStepButton = new QPushButton(centralWidget);
        oneStepButton->setObjectName(QStringLiteral("oneStepButton"));

        verticalLayout_2->addWidget(oneStepButton);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        nEpochsButton = new QPushButton(centralWidget);
        nEpochsButton->setObjectName(QStringLiteral("nEpochsButton"));

        horizontalLayout->addWidget(nEpochsButton);

        nEpochsSpinBox = new QSpinBox(centralWidget);
        nEpochsSpinBox->setObjectName(QStringLiteral("nEpochsSpinBox"));
        nEpochsSpinBox->setMinimum(1);
        nEpochsSpinBox->setMaximum(100);
        nEpochsSpinBox->setSingleStep(1);
        nEpochsSpinBox->setValue(1);

        horizontalLayout->addWidget(nEpochsSpinBox);


        verticalLayout_2->addLayout(horizontalLayout);

        solveButton = new QPushButton(centralWidget);
        solveButton->setObjectName(QStringLiteral("solveButton"));

        verticalLayout_2->addWidget(solveButton);


        mainLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(mainLayout);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 593, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        openFileButton->setText(QApplication::translate("MainWindow", "Open file", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "Decay type", 0));
        staticButton->setText(QApplication::translate("MainWindow", "Static", 0));
        linearButton->setText(QApplication::translate("MainWindow", "Linear", 0));
        exponentialButton->setText(QApplication::translate("MainWindow", "Exponential", 0));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Influence type", 0));
        staticInfluenceButton->setText(QApplication::translate("MainWindow", "Static", 0));
        exponentialInfluenceButton->setText(QApplication::translate("MainWindow", "Exponential", 0));
        label->setText(QApplication::translate("MainWindow", "Estimated number of epochs", 0));
        oneStepButton->setText(QApplication::translate("MainWindow", "1 step", 0));
        nEpochsButton->setText(QApplication::translate("MainWindow", "N epochs", 0));
        solveButton->setText(QApplication::translate("MainWindow", "Solve", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
