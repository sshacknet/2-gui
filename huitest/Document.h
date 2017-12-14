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

    inline bool operator==(Document doclist) const { return (word == doclist.word); }
    inline bool operator>(Document doclist) const { return (word > doclist.word); }
    inline bool operator<(Document doclist) const { return (word < doclist.word); }
};

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

/*
 *  InvertedIndexDocument
 *  将关键字以键值对的形式存储在AVLTree中
 *  
 *
 */
