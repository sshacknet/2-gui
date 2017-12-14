#pragma once

#include <vector>
#include "String.h"
#include "CharString.h"

class HTMLElement
{
public:
    String content;
    String type;
    String classes;
    String id;
    
    bool is_start_token;
    int _type;
    HTMLElement() { _type = -1; }
    HTMLElement(const HTMLElement& from)
    {
        content = from.content;
        type = from.type;
        classes = from.classes;
        id = from.id;
        is_start_token = from.is_start_token;
        _type = from._type;
    }
    HTMLElement& operator=(const HTMLElement& from)
    {
        content = from.content;
        type = from.type;
        classes = from.classes;
        id = from.id;
        is_start_token = from.is_start_token;
        _type = from._type;
        return *this;
    }
    ~HTMLElement() { destory(); };
    void destory()
    {
        content.destory();
        type.destory();
        classes.destory();
        id.destory();
    }
};
//存储解析的单个网页信息
class PageInfo
{
public:
    PageInfo()
    {
    }

    PageInfo(PageInfo const& info)
    {
        url = info.url;
        title = info.title;
        author = info.author;
        category = info.category;
        subcategory = info.subcategory;
        type = info.type;
        content = info.content;
        time = info.time;
    }

    std::string url;
    String title;//标题
    String author;//作者
    String category;//大类
    String subcategory;//小类
    String type;//发帖类型
    String content;//发帖内容
    String time;

public:
    void show();
};

inline void PageInfo::show()
{
    std::cout << "\n--------网页信息---------\n";
    std::cout << "标    题: ";
    title.output();
    std::cout << std::endl;
    std::cout << "作    者: ";
    author.output();
    std::cout << std::endl;
    std::cout << "发帖大类: ";
    category.output();
    std::cout << std::endl;
    std::cout << "发帖小类: ";
    subcategory.output();
    std::cout << std::endl;
//    std::cout << "发帖类型: ";
//    type.output();
//    std::cout << std::endl;
    std::cout << "发帖内容: ";
    content.output();
    std::cout << std::endl;
    std::cout << "发帖时间: ";
    time.output();
    std::cout << std::endl;
    std::cout << "\n-------------------------\n";
}
