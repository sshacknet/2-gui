#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_guitest.h"

class guitest : public QMainWindow
{
    Q_OBJECT

public:
    guitest(QWidget *parent = Q_NULLPTR);

private:
    Ui::guitestClass ui;
};
