#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
class Error
{
public:

    std::string _msg;


    Error(const std::string msg)
    {
        _msg = msg;
    }

    Error() = default;

    void what();
};

inline void Error::what()
{
    std::cout << "Error message: [" << _msg << "]" << std::endl;;
    abort();
}
