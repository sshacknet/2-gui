#pragma once
#include "Error.h"
#include <memory>

template <class KeyType>
class StackNode
{
public:
    StackNode(const KeyType& key) : __key(key) {}

    ~StackNode() { }

    inline KeyType value() { return __key; }
    inline std::shared_ptr<StackNode> next() { return __next; }
    inline void setNext(std::shared_ptr<StackNode> node) { __next = node; }

private:
    KeyType __key;
    std::shared_ptr<StackNode> __next;
};


template <class KeyType>
class Stack
{
public:
    Stack() : head(nullptr), __size(0) {}

    ~Stack() { }

    KeyType top();
    bool isEmpty();
    void push(const KeyType key);
    unsigned int size();
    void pop();
private:
    std::shared_ptr<StackNode<KeyType>> head;
    int __size;
};

template <class KeyType>
KeyType Stack<KeyType>::top()
{
    if (__size > 0)
    {
        return head->value();
    }
    throw Error("get top when stack is empty");
}

template <class KeyType>
bool Stack<KeyType>::isEmpty()
{
    return __size == 0;
}

template <class KeyType>
void Stack<KeyType>::push(KeyType key)
{
    auto p = std::make_shared<StackNode<KeyType>>(key);
    p->setNext(head);
    head = p;
    __size++;
}

template <class KeyType>
unsigned Stack<KeyType>::size()
{
    return __size;
}

template <class KeyType>
void Stack<KeyType>::pop()
{
    auto current_head = head;
    if (head)
    {
        head = head->next();
        __size--;
        // delete current_head;
    }
}
