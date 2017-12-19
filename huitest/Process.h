#pragma once
#include "AVLTree.hpp"
#include "CharString.h"
#include "Document.h"
#include "Error.h"
#include "HTMLElement.h"
#include <QtNetwork>
#include <QObject>
#include <QString>
#include <QFile>
#include <thread>
#include <mutex>
#include <chrono>
#include "StringHashTable.h"
#include <QProgressBar>
#include <QTime>
/*
 * 设置类，用于控制程序默认行为
 */
class config
{
public:
    bool DOWNLOAD_CLOUD_DIC;//是否加载云词典
    bool DOWNLOAD_HTML;//是否下载网页
    bool LOAD_AVL; //使用AVL树而不是哈希表
    config(): DOWNLOAD_CLOUD_DIC(false), DOWNLOAD_HTML(false),LOAD_AVL(true) {}

    config(config& from)
    {
        DOWNLOAD_CLOUD_DIC = from.DOWNLOAD_CLOUD_DIC;
        DOWNLOAD_HTML = from.DOWNLOAD_HTML;
        LOAD_AVL = from.LOAD_AVL;
    }
};
extern config g_conf;//全局设置
extern QStringList g_urllist;//url列表
extern LinkList<PageInfo> g_pageinfo_list;//网页信息链表
extern InvertedIndexDocument g_inverted_index_document;//倒排文档
extern int constructTime;//建立词典时间
class Process:public QObject
{
    Q_OBJECT
public:
    static AVLTree<String> dic_avl;
    static StringHashTable dic_hash;
    static void setconfig(config _conf);
    static void init();
    static void DownloadHTML(QStringList urllist);
    static AVLTree<String> initDictionary_avl();
    static StringHashTable initDictionary_hash();
    static LinkList<String> divideWords(String words, AVLTree<String> dic);
    static LinkList<String> divideWords(String words, StringHashTable dic);
    static PageInfo extractInfo_onepage(std::string filename);
    static LinkList<PageInfo> extractInfo(int pagenumbers);
    static void process();
    static InvertedIndexDocument generateDocument(LinkList<PageInfo>);
    static void cURL(const QString url, const QString filename, bool is_download_html = true);
    static void ClearFile(int pagenumber);
private:
    static QWidget* HintWindow;
    static QProgressBar* progress;
};
