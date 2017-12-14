#include "Document.h"


void InvertedIndexDocument::add(String word, int urlid)
{
    if(documenttree.search(Document(word)))
    {
        edit(documenttree.search(Document(word)), urlid);
    }
    else
    {
        Document newdocument(word);
        newdocument.update(urlid);
        documenttree.insert(newdocument);
    }

}

Document InvertedIndexDocument::search(String word)
{
    if(documenttree.search(Document(word)))
    {
        return documenttree.search(Document(word))->key;
    }
    return Document();
}

void InvertedIndexDocument::edit(std::shared_ptr<AVLTreeNode<Document>>  doclist, int urlid)
{
    doclist->key.update(urlid);
}

void InvertedIndexDocument::remove()
{
    //todo
}
