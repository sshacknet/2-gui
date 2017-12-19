#include "HTMLparser.h"
#include <locale>
#include "CharString.h"
#include <stdio.h>
#include <QDebug>
#include <QFile>
#include <QDataStream>
using namespace std;

#define  TITLE 3
#define DIV 25
#define P 29
#define TD 18
#define EM 27

String token[40];
bool token_flag = false;
int token_number = 38;
const char* GBK_LOCALE_NAME = ".936"; //GBK在windows下的locale name
wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> Conver_GBK(
    new std::codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));

void init_token()
{
    token[0] = String(L"html");
    token[1] = String(L"head");
    token[2] = String(L"meta");//*
    token[3] = String(L"title");
    token[4] = String(L"base");//*
    token[5] = String(L"script");
    token[6] = String(L"body");
    token[7] = String(L"a");
    token[8] = String(L"strong");
    token[9] = String((L"ul"));
    token[10] = String(L"link");//*
    token[11] = String(L"span");
    token[12] = String(L"dl");
    token[13] = String(L"dd");
    token[14] = String(L"input");//*
    token[15] = String(L"dt");//
    token[16] = String(L"table");
    token[17] = String(L"tr");
    token[18] = String(L"td");
    token[19] = String(L"button");
    token[20] = String(L"style");
    token[21] = String(L"li");
    token[22] = String(L"img");//*
    token[23] = String(L"label");
    token[24] = String(L"form");
    token[25] = String(L"div");
    token[26] = String(L"br");//*
    token[27] = String(L"em");
    token[28] = String(L"h1");
    token[29] = String(L"p");
    token[30] = String(L"th");//*
    token[31] = String(L"font");
    token[32] = String(L"h3");
    token[33] = String(L"iframe");
    token[34] = String(L"b");
    token[35] = String(L"h4");
    token[36] = String(L"blockquote");
    token[37] = String(L"ignore_js_op");
}

HTMLparser::HTMLparser(string filename)
{
    if (!token_flag)
    {
        init_token();
        token_flag = true;
    }
    qDebug() << "parsering " << QString::fromStdString(filename);
    load(filename);
}

/**
 * \brief 从GBK编码的网页文件中读取文本并转换为UTF8
 * \param filename 文件名
 */
void HTMLparser::load(std::string filename)
{
//    FILE* fp;
//    fp = fopen(("temp/" + filename).c_str(), "r");
//    if (!fp)
//        throw Error("Can not open" + filename);
//    fseek(fp, 0, SEEK_END);
//    int fileSize = ftell(fp);
//    char* buf = new char[fileSize+1];
//    memset(buf, 0, fileSize + 1);
//
//    fseek(fp, 20, SEEK_SET);
//    fread(buf, sizeof(char), fileSize, fp);
//    fclose(fp);
//
//    std::wstring wDst = Conver_GBK.from_bytes(buf);
//    delete[] buf;
//    buf = nullptr;
    QFile f(QString::fromStdString("temp/" + filename));
    if (!f.open(QIODevice::ReadOnly))
        throw Error("cant open html file");
    QDataStream stream(&f);
    auto wDst = QString(f.readAll()).toStdWString();
    html = String(wDst);

}

/**
 * \brief 使用栈将HTML文本划分成dom节点,将出栈后结果保存在result
 */
void HTMLparser::toknize()
{
    String t;
    wchar_t* __html = html.c_str();
    wchar_t* buffer;
    wchar_t* token = nullptr;
    LinkList<String> tokens;
    try
    {
        token = wcstok(__html, L"<", &buffer);
        while (token)
        {
            String tmp(token);
            t = String(L"<").concat(tmp);
            tokens.push_back(t);
            if (tokens.size() > 700)
                break;
            token = wcstok(nullptr, L"<", &buffer);
        }
    }
    catch (Error& error)
    {
        error.what();
    }

    auto current_node = tokens.head;


    try
    {
        while (!t.empty())
        {
            if (!current_node)
                return;
            t = current_node->_data;
            if (t[1] == L'!')
            {
                current_node = current_node->_next;
                continue;
            }
            HTMLElement e = parse_token(t);
            if (e.is_start_token)
            {
                if (!is_not_paired(e))
                {
                    parse_content(e, t);
                    doms.push(e);
                }
                else
                {
                    result.push_back(e);
                }
            }
            else//是结束标签
            {
                HTMLElement top;
                top = doms.top();
                if (&top)
                {
                    //栈顶元素与当前元素类型相同，将栈顶元素出栈，添加进结果链表中
                    if (top._type == e._type)
                    {
                        top = doms.top();
                        doms.pop();
                        result.push_back(top);
                        if (result.size() > 400)
                            return;
                    }
                }
            }


            current_node = current_node->_next;
        }
    }
    catch (Error& e)
    {
        e.what();
    }

}


/**
 * \brief 判断该html节点是否是自闭合的
 */
bool HTMLparser::is_not_paired(HTMLElement e)
{
    auto t = e._type;
    return t == 2 || t == 4 || t == 10 || t == 14 || t == 22 || t == 26 || t == 30;
}

/**
 * \brief 从字符串解析出单个HTML节点并判断类型
 */
HTMLElement HTMLparser::parse_token(String s)
{
    HTMLElement e;
    // 处理结束节点的情况
    if (s[1] == L'/')
    {
        e.is_start_token = false;
        for (int i = 2; i < s.size(); i++)
        {
            if (s[i] == L'>')
            {
                e.type = s.substr(2, i);
                for (int j = 0; j < token_number; j++)
                {
                    if (e.type == token[j])
                    {
                        e._type = j;
                        break;
                    }
                }
                return e;
            }
        }
    }
    else//处理是开始节点的情况
    {
        e.is_start_token = true;
        for (int i = 1; i < s.size(); i++)
        {
            if (s[i] == L' ' || s[i] == L'>' || s[i] == L'/')
            {
                e.type = s.substr(1, i);
                for (int j = 0; j < token_number; j++)
                {
                    if (e.type == token[j])
                    {
                        e._type = j;

                        break;
                    }
                }
                return e;
            }
        }
    }
}

/**
 * \brief 解析html节点e的内容
 */
void HTMLparser::parse_content(HTMLElement& e, String s)
{
    int start = 0;
    for (int i = 0; i < s.size(); i++)
    {
        if (s[i] == L'>')
        {
            start = i + 1;
            break;
        }
    }
    //提取其内容
    if (s.size() - start > 0)
    {
        e.content = s.substr(start, s.size());
    }

    //提取 class 信息
    if (s.indexof(String(L"class")) != s.size())
    {
        start = s.indexof(String(L"class"));
        int j;
        for (j = start + 7; j < s.size(); j++)
        {
            if (s[j] == L'\"')
                break;
        }
        e.classes = s.substr(start + 7, j);
    }
    //提取 id 信息
    //    if (s.indexof(String(L"id")) != s.size())
    //    {
    //        int start = s.indexof(String(L"id"));
    //        int j;
    //        for (j = start + 7; j < s.size(); j++)
    //        {
    //            if (s[j] == L'\"')
    //                break;
    //        }
    //        e.id = s.substr(start + 4, j);
    //    }
}

/**
 * \brief 从 result 中提取网页信息
 */
PageInfo HTMLparser::parse()
{
    PageInfo info;
    //网页信息所在处的class名
    String class_z(L"z");
    String class_authi(L"authi");
    String class_t_f(L"t_f");
    String class_ts(L"ts z h1");
    bool author_found = false;
    bool tf_found = false;
    bool time_found = false;
    try
    {
        for (int i = 0; i < result.size(); i++)
        {
            if (result[i]._type == DIV && result[i].classes == class_authi && !author_found)
            {
                info.author = result[i - 1].content;
                author_found = true;
            }
            if (result[i]._type == DIV && result[i].classes == class_authi && result[i - 5]._type == EM && !
                time_found)
            {
                info.time = result[i - 5].content;
                time_found = true;
            }

            if (result[i]._type == TD && result[i].classes == class_t_f && !tf_found)
            {
                //特殊处理编码问题 如果正常则有三层 否则有四层并且需要转码
                String tmp;

                if (result[i - 1]._type == P)
                {
                    tmp = result[i - 1].content;
                }
                else if (result[i - 2]._type == P)
                {
                    tmp = result[i - 2].content;
                }
                else if (result[i - 3]._type == P)
                {
                    tmp = result[i - 3].content;
                }
                else
                {
                    tmp = result[i].content;
                }
                int real_content_start;
                for (real_content_start = 0; real_content_start < tmp.size(); real_content_start++)
                {
                    if (tmp[real_content_start] != L' ' && tmp[real_content_start] != L'\n' && tmp[real_content_start]
                        != L'\t')
                        break;
                }
                if (real_content_start < tmp.size())
                {
                    info.content = tmp.substr(real_content_start, tmp.size());
                    info.content = decode(info.content);
                }
                tf_found = true;
            }
            if (result[i]._type == DIV && result[i].classes == class_ts)
            {
                info.type = result[i - 5].content;
            }
            if (result[i]._type == DIV && result[i].classes == class_z)
            {
                info.category = result[i - 5].content;
                info.subcategory = result[i - 3].content;
                info.title = result[i - 1].content;
            }
            if (author_found && time_found && tf_found && !info.type.empty() && !info.category.empty())
            {
                //cout << "结果：" << i << "/" << result.size() << endl;
                return info;
            }
        }
    }
    catch (Error& e)
    {
        e.what();
    }

    return info;
}

/**
 * \brief 清理内存
 */
void HTMLparser::destory()
{
    html.destory();
}
