#pragma once
#include "LinkList.hpp"
#include "CharString.h"
#include "AVLTree.hpp"

class QueryInfo
{
public:
    int urlID;
    int occurTimes;
    QueryInfo(int urlid) : urlID(urlid), occurTimes(1) {}
    QueryInfo() = default;
    QueryInfo(int urlid, int occurtimes):urlID(urlid),occurTimes(occurtimes){}
    inline bool operator==(QueryInfo doc) { return urlID == doc.urlID; }
};


/**
 * \brief 文档链表类，使用链表存储关键词的出现信息
 */
class Document
{
public:
    String word;

    Document(const Document& doc)
    {
        word = doc.word;
        documents = doc.documents;
    }
    Document() = default;
    Document(String word): word(word) {}
    LinkList<QueryInfo> documents;

    void add(int urlid);
    void edit(int urlid, int occurtimes);
    void update(int urlid)
    {
        if (documents.contains(QueryInfo(urlid)))
        {
            auto current = documents.head;
            while (current)
            {
                if (current->_data.urlID == urlid)
                {
                    current->_data.occurTimes++;
                    break;
                }
                current = current->_next;
            }
        }
        else
        {
            documents.insert(QueryInfo(urlid));
        }
    }

    /*
     * 重载运算符，以便能放入AVLTree中
     */
    inline bool operator==(Document doclist) const { return (word == doclist.word); }
    inline bool operator>(Document doclist) const { return (word > doclist.word); }
    inline bool operator<(Document doclist) const { return (word < doclist.word); }
};

/**
 * \brief 倒排文档，将关键字以键值对的形式存储在AVLTree中
 */
class InvertedIndexDocument
{
public:
    InvertedIndexDocument() = default;
    InvertedIndexDocument(InvertedIndexDocument& from): documenttree(from.documenttree) {}
    void add(String word, int urlid);
    Document search(String word);
    void edit(std::shared_ptr<AVLTreeNode<Document>> doclist, int urlid);
    void remove();
    inline AVLTree<Document> getroot() { return  documenttree; }
private:
    AVLTree<Document> documenttree;
};


