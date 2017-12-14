/********************************************************************************
** Form generated from reading UI file 'guitest.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUITEST_H
#define UI_GUITEST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_guitestClass
{
public:
    QWidget *centralWidget;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *guitestClass)
    {
        if (guitestClass->objectName().isEmpty())
            guitestClass->setObjectName(QStringLiteral("guitestClass"));
        guitestClass->resize(600, 400);
        centralWidget = new QWidget(guitestClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(70, 10, 321, 31));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(430, 10, 101, 31));
        guitestClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(guitestClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 23));
        guitestClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(guitestClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        guitestClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(guitestClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        guitestClass->setStatusBar(statusBar);

        retranslateUi(guitestClass);

        QMetaObject::connectSlotsByName(guitestClass);
    } // setupUi

    void retranslateUi(QMainWindow *guitestClass)
    {
        guitestClass->setWindowTitle(QApplication::translate("guitestClass", "guitest", nullptr));
        pushButton->setText(QApplication::translate("guitestClass", "Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class guitestClass: public Ui_guitestClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUITEST_H
