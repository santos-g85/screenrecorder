/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *stopbtn;
    QPushButton *startbtn;
    QLabel *label;
    QMenuBar *menubar;
    QMenu *menuScreen_Recorder;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        stopbtn = new QPushButton(centralwidget);
        stopbtn->setObjectName("stopbtn");
        stopbtn->setGeometry(QRect(360, 170, 83, 29));
        startbtn = new QPushButton(centralwidget);
        startbtn->setObjectName("startbtn");
        startbtn->setGeometry(QRect(200, 170, 83, 29));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(200, 89, 241, 61));
        label->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 0, 0);"));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuScreen_Recorder = new QMenu(menubar);
        menuScreen_Recorder->setObjectName("menuScreen_Recorder");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuScreen_Recorder->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        stopbtn->setText(QCoreApplication::translate("MainWindow", "stop", nullptr));
        startbtn->setText(QCoreApplication::translate("MainWindow", "start", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "start", nullptr));
        menuScreen_Recorder->setTitle(QCoreApplication::translate("MainWindow", "Screen Recorder", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
