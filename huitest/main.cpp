#include "guitest.h"
#include <QtWidgets/QApplication>
#include <QTextCodec>
#include <QDebug>
#include "Process.h"
#include <locale>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    setlocale(LC_ALL, "chs");
    Process::init();
    Process::process();
    qDebug() << "process complete!" << endl;
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));
    guitest w;
    w.show();
    return a.exec();
}
