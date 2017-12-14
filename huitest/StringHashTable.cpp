
#include "StringHashTable.h"

/**
 * \brief 简易的字符串哈希函数
 * \param str 字符串
 * \return 哈希值，范围在[0,HASH_TABLE_SIZE)
 */
unsigned int StringHashTable::hashfunction(String str)
{
    unsigned int hash = 0;
    int seed = 233;
    for (int i = 0; i < str.size(); i++)
    {
        hash = hash * seed + str[i];
        hash = hash % HASH_TABLE_SIZE;
    }
    return hash;
}

StringHashTable::StringHashTable()
{
    data = new LinkList<String>[HASH_TABLE_SIZE];
    if (!data)
    {
        throw Error("Init hashtable fail");
    }
    //memset(data, NULL, HASH_TABLE_SIZE);
}

StringHashTable::StringHashTable(const StringHashTable& sht)
{
    data = sht.data;
}

/**
 * \brief 向哈希表中插入元素
 */
void StringHashTable::insert(String str)
{
    auto hashcode = hashfunction(str);
    if (!data[hashcode].contains(str))
    {
        data[hashcode].insert(str);
    }
}


/**
 * \brief 操作符[] 判断字符串str是否在哈希表中
 */
bool StringHashTable::operator[]( String str)
{
    auto hashcode = hashfunction(str);
    return data[hashcode].contains(str);
}
