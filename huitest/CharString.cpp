#include "CharString.h"
#include <wchar.h>
#include <string>
using std::cout;
using std::endl;
String::String( CharType* str): tok_point(0)
{
    int size = (str ? wcslen(str) + 1 : 1);
    data = new CharType[size];
    if (str)
    {
        wcscpy(data, str);
    }
    else
    {
        data[0] = '\0';
    }
}
String::String(std::wstring str): tok_point(0)
{
    int size = str.size() + 1;
    data = new CharType[size];
    wcscpy(data, str.data());
}

String::String(String& str): tok_point(0)
{
    data = new CharType[str.size() + 1];
    wcscpy(data, str.data);
}

/**
 * \brief 赋值(assign)
 */
String& String::operator=(const String& str)
{
    if (this != &str)
    {
        destory();
        data = new CharType[wcslen(str.data) + 1];
        wcscpy(data, str.data);
    }

    return *this;
}

String::~String()
{
    destory();
}

/**
 * \brief 获取字符串下标为index的字符的引用
 */
CharType& String::operator[](SizeType index)
{
    if (index >= this->size() || index < 0)
    {
        throw Error("String::operator[]: index out of range");
    }
    return data[index];
}

bool String::operator>(const String str) const
{
    return wcscmp(data, str.data) > 0 ? true : false;
}

/**
 * \brief 获取字符串长度
 */
SizeType String::size()
{
    return wcslen(data);
}

/**
 * \brief 判断两个字符串是否相等
 * \param str 
 * \return 
 */
bool String::operator==(const String str) const
{
    return wcscmp(data, str.data) == 0;
}

bool String::operator<(const String str) const
{
    return wcscmp(data, str.data) < 0 ? true : false;
}

/**
 * \brief 返回下标从start_p到end_p的子字符串，不包括end_p!!
 */
String String::substr(SizeType start_p, SizeType end_p)
{
    if (start_p < 0 || start_p >= size() || end_p < 0 || end_p > size() || start_p >= end_p)
    {
        std::cout << "Traceback: start_p= " << start_p << "  end_p= " << end_p << std::endl;
        throw Error("String::substr: index error");
    }
    CharType* subdata = new CharType[end_p - start_p + 1];
    int j = 0;
    for (int i = start_p; i < end_p; i++)
    {
        subdata[j] = data[i];
        j++;
    }
    //memcpy(subdata, &data[start_p], end_p - start_p);
    subdata[end_p - start_p] = L'\0';
    String tmp(subdata);
    delete[] subdata;
    return tmp;
}

/**
 * \brief 使用kmp算法进行字符串匹配
 * \param sub_str 待匹配的子串
 * \return 子串第一次出现的位置，未出现则返回this->size();
 */
SizeType String::indexof(String& sub_str)
{
    if (sub_str.empty())
    {
        return size();
    }

    int* next = new int[sub_str.size() + 1];
    int j = -1;
    next[0] = -1;
    for (int i = 1; i < sub_str.size(); ++i)
    {
        while (j > -1 && sub_str[j + 1] != sub_str[i])
            j = next[j];
        if (sub_str[j + 1] == sub_str[i])
            ++j;
        next[i] = j;
    }
    j = -1;
    for (int i = 0; i < size(); ++i)
    {
        while (j > -1 && sub_str[j + 1] != data[i])
            j = next[j];
        if (sub_str[j + 1] == data[i])
            ++j;
        if (j == sub_str.size() - 1)
        {
            delete[] next;
            return i - sub_str.size() + 1;
        }
    }
    delete[] next;
    return size();
}

/**
 * \brief 字符串连接
 * \param str 待连接的字符串
 * \return 连接后的字符串,注意此函数不会改变两个连接的字符串的内容
 */
String String::concat(String str)
{
    CharType* concatdata = new CharType[size() + str.size() + 1];
    int i, j;
    for (i = 0; i < size(); i++)
    {
        concatdata[i] = data[i];
    }
    for (j = i; j < size() + str.size(); j++)
    {
        concatdata[j] = str[j - i];
    }
    concatdata[size() + str.size()] = L'\0';
    String concatstring(concatdata);
    delete[] concatdata;
    return concatstring;
}

/**
 * \brief 判断字符串是否为空
 */
bool String::empty()
{
    return size() == 0;
}

/**
 * \brief 字符串分割函数，返回分隔符到下一个分割符之间的子字符串
 * \param delim 分隔符
 * \param first 是否是第一次执行
 * \return 分割出的子字符串，不可继续分割时返回空字符串
 */
String String::strtok(CharType delim, bool first)
{
    if (first)//第一次执行，需要查找起始位置
    {
        int start = 0, end = 0, i;
        for (i = 0; i < size(); i++)
        {
            if (data[i] == delim)
            {
                start = i;
                break;
            }
        }
        for (int j = 1 + i; j < size(); j++)
        {
            if (data[j] == delim)
            {
                end = j;
                tok_point = j;
                break;
            }
        }
        String tmp;
        if (end > start)
            tmp = substr(start, end);
        return tmp;
    }
    else
    {
        int end = 0;
        int i;
        for (i = tok_point + 1; i < size(); i++)
        {
            if (data[i] == delim)
            {
                end = i;
                break;
            }
        }
        

        String tmp;
        if (i == size() && tok_point < size() && end == 0)
        {
            tmp = substr(tok_point, size());
            tok_point = size();
            return tmp;
        }
        if (end > tok_point)
        {
            tmp = substr(tok_point, end);
            tok_point = end;
        }
        return tmp;
    }
}

/**
 * \brief 获取wcahr_t[]的数据
 */
wchar_t* String::c_str() const
{
    return data;
}

std::wstring String::toWstring() const
{
    return std::wstring(data);
}


/**
 * \brief 释放内存
 */
void String::destory()
{
    if (data)
    {
        delete[] data;
        data = nullptr;
    }
}

/**
 * \brief 将src中的unicode编码字符转为utf-8编码
 */
String decode(String src)
{
    //去除“&nbsp;”(空格)
    String tag2(L"&nbsp");

    while (true)
    {
        int spaceindex = src.indexof(tag2);
        if (spaceindex == src.size())
            break;
        for (int i = spaceindex; i < spaceindex + 7; i++)
        {
            src[i] = L' ';
        }
    }

    String result;
    String tag_end(L";");
    String tag_front(L"&");

    unsigned int first_tok = src.indexof(tag_front);
    cout << first_tok;
    if (first_tok > 0)
    {
        String tmp;
        tmp = src.substr(0, first_tok);
        result = result.concat(tmp);
    }

    String temp;
    temp = (src.strtok(L'&', true));

    while (!temp.empty())
    {
        int ans = 0;
        for (int i = 2; i < temp.indexof(tag_end); i++)
        {
            ans = ans * 10 + temp[i] - L'0';
        }

        wchar_t ch[] = L" ";
        ch[0] = static_cast<wchar_t>(ans);


        String tmp;
        tmp = String(ch);
        result = result.concat(tmp);
        if (temp.indexof(tag_end) + 1 < temp.size())
            result = result.concat(temp.substr(temp.indexof(tag_end) + 1, temp.size()));
        temp = src.strtok(L'&');
    }


    return result;
}


void String::output()
{
    wprintf(L"%ls", data);
}

/**
 * \brief 去除中文标点符号
 */
String removeChineseSymbol(String str)
{
    String symbol[20];
    symbol[0] = String(L"，");
    symbol[1] = String(L"；");
    symbol[2] = String(L"·");
    symbol[3] = String(L"！");
    symbol[4] = String(L"？");
    symbol[5] = String(L"￥");
    symbol[6] = String(L"。");
    symbol[7] = String(L"～");
    symbol[8] = String(L"：");
    symbol[9] = String(L"‘");
    symbol[10] = String(L";");
    symbol[11] = String(L" ");
    symbol[12] = String(L",");
    symbol[13] = String(L"、");
    symbol[14] = String(L"’");
    symbol[15] = String(L"/");
    symbol[16] = String(L".");
    symbol[17] = String(L"“");
    symbol[18] = String(L"”");
    String result;
    for (int i = 0; i < str.size(); i++)
    {
        bool is_symbol = false;
        for (int j = 0; j < 19; j++)
        {
            if (str.substr(i, i + 1) == symbol[j])
            {
                is_symbol = true;
                break;
            }
        }
        if (!is_symbol)
            result = result.concat(str.substr(i, i + 1));
    }
    return result;
}

bool is_letter_or_number(CharType c)
{
    return (c >= L'0' && c <= L'9') || (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c == L'-') || (c ==
        L'——');
}
