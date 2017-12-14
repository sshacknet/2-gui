#include "guitest.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    guitest w;
    w.show();
    return a.exec();
}
