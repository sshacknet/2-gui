#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_guitest.h"
#include <QTextBrowser>

class guitest : public QMainWindow
{
Q_OBJECT

public:
    guitest(QWidget* parent = Q_NULLPTR);
    void resizeEvent(QResizeEvent* event) override;
public slots:
    void search();
private:
    Ui::guitestClass ui;
    QTextBrowser* result;
};
