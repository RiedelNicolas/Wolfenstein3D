/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "itemList.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QScrollArea *mapArea;
    QWidget *nada;
    ItemList *itemList;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QFormLayout *formLayout;
    QPushButton *button;
    QLabel *label;
    QSpinBox *spinY;
    QLabel *label_2;
    QSpinBox *spinX;
    QLabel *label_3;
    QLineEdit *nameLabel;
    QFrame *trashFrame;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(896, 520);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        mapArea = new QScrollArea(centralwidget);
        mapArea->setObjectName(QString::fromUtf8("mapArea"));
        mapArea->setGeometry(QRect(250, 20, 641, 461));
        mapArea->setWidgetResizable(true);
        nada = new QWidget();
        nada->setObjectName(QString::fromUtf8("nada"));
        nada->setGeometry(QRect(0, 0, 639, 459));
        mapArea->setWidget(nada);
        itemList = new ItemList(centralwidget);
        itemList->setObjectName(QString::fromUtf8("itemList"));
        itemList->setGeometry(QRect(0, 0, 221, 201));
        horizontalLayoutWidget = new QWidget(centralwidget);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(0, 210, 221, 141));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        button = new QPushButton(horizontalLayoutWidget);
        button->setObjectName(QString::fromUtf8("button"));

        formLayout->setWidget(3, QFormLayout::FieldRole, button);

        label = new QLabel(horizontalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        spinY = new QSpinBox(horizontalLayoutWidget);
        spinY->setObjectName(QString::fromUtf8("spinY"));
        spinY->setMaximum(1000);

        formLayout->setWidget(0, QFormLayout::FieldRole, spinY);

        label_2 = new QLabel(horizontalLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        spinX = new QSpinBox(horizontalLayoutWidget);
        spinX->setObjectName(QString::fromUtf8("spinX"));
        spinX->setMaximum(1000);

        formLayout->setWidget(1, QFormLayout::FieldRole, spinX);

        label_3 = new QLabel(horizontalLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        nameLabel = new QLineEdit(horizontalLayoutWidget);
        nameLabel->setObjectName(QString::fromUtf8("nameLabel"));

        formLayout->setWidget(2, QFormLayout::FieldRole, nameLabel);


        horizontalLayout->addLayout(formLayout);

        trashFrame = new QFrame(centralwidget);
        trashFrame->setObjectName(QString::fromUtf8("trashFrame"));
        trashFrame->setGeometry(QRect(0, 360, 221, 101));
        trashFrame->setFrameShape(QFrame::StyledPanel);
        trashFrame->setFrameShadow(QFrame::Raised);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 896, 23));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        button->setText(QCoreApplication::translate("MainWindow", "Aplicar", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Altura", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Ancho", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "dough", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H