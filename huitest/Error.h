#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <QDebug>
class Error
{
public:
    std::string msg;
    Error(const std::string msg): msg(msg) {}
    Error() = default;
    void what();
};

inline void Error::what()
{
    qDebug() << "Error message: [" << QString::fromStdString(msg) << "]";
    abort();
    system("pause");
}
