#pragma once
#include "AVLTree.hpp"
#include "CharString.h"
#include "Document.h"
#include "Error.h"
#include "HTMLElement.h"
#include <QtNetwork>
#include <QString>
#include <QFile>
#include <thread>
#include <mutex>
#include <chrono>
#include "StringHashTable.h"
class config
{
public:
    bool DOWNLOAD_CLOUD_DIC;
    bool DOWNLOAD_HTML;
    config(): DOWNLOAD_CLOUD_DIC(false), DOWNLOAD_HTML(false) {}

    config(config& from)
    {
        DOWNLOAD_CLOUD_DIC = from.DOWNLOAD_CLOUD_DIC;
        DOWNLOAD_HTML = from.DOWNLOAD_HTML;
    }
};
extern config g_conf;
extern QStringList g_urllist;
extern LinkList<PageInfo> pageinfo_list;
extern InvertedIndexDocument inverted_index_document;
class Process
{
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
};
