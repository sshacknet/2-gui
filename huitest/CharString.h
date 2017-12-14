#pragma once

#include "Error.h"
#include<iostream>
typedef wchar_t CharType;
typedef unsigned int SizeType;

class String
{
private:
    CharType* data;
    SizeType tok_point;
public:
    String(CharType* str = nullptr);
    String(std::wstring);
    String(String& str);
    ~String();
    void destory();
    String& operator=(const String& str);

public:
    CharType& operator[](SizeType index);

    bool operator==(const String str) const;
    bool operator<(const String str) const;
    bool operator>(const String str) const;
    SizeType size();

    String substr(SizeType start_p, SizeType end_p);

    SizeType indexof(String& str);

    String concat(String str);

    bool empty();

    String strtok(CharType delim, bool first = false);

    wchar_t* c_str() const;
    std::wstring toWstring() const;
    void output();
};

String decode(String src);
String removeChineseSymbol(String str);
bool is_letter_or_number(CharType c);
