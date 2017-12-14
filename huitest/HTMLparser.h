#pragma once

#include "HTMLElement.h"
#include "LinkList.hpp"
#include <stack>
#include "CharString.h"

class HTMLparser
{
private:
    String html;
    std::stack<HTMLElement> doms;
    LinkList<HTMLElement> result;
    std::string _filename;
    
public:
    HTMLparser(std::string filename);

    void load(std::string filename);

    void toknize();

    void standardized();

    static bool is_not_paired(HTMLElement e);

    HTMLElement parse_token(String s);

    static void parse_content(HTMLElement& e, String s);

    PageInfo parse();

    void destory();

    virtual ~HTMLparser()
    {
        destory();
    }
};

