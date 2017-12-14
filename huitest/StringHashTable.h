#pragma once
#include "Error.h"
#include "LinkList.hpp"
#include "CharString.h"
#define HASH_TABLE_SIZE  100007


class StringHashTable
{
private:
    LinkList<String>* data;
public:
    StringHashTable();
    StringHashTable(const StringHashTable& sht);

    static unsigned int hashfunction(String str);
    void insert(String str);
    bool operator[](String str);
};
