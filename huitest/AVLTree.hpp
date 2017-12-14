﻿#pragma once
#include <memory>
#define max(a, b) ((a)>(b)?(a):(b))
/*
 * 二叉平衡树树节点
 */
template <class KeyType>
class AVLTreeNode
{
public:
    AVLTreeNode(KeyType key): rchild(nullptr), lchild(nullptr), height(0), key(key) {}
    std::shared_ptr<AVLTreeNode<KeyType>> rchild, lchild;//子节点
    int height;//高度
    KeyType key;//关键字
};

/*
 * 二叉平衡树，使用shared_ptr进行自动内存管理
 */
template <class KeyType>
class AVLTree
{
private:
    std::shared_ptr<AVLTreeNode<KeyType>> root;//根节点

public:
    AVLTree(): root(nullptr) {}
    AVLTree(const AVLTree& from) { root = from.root; }
    std::shared_ptr<AVLTreeNode<KeyType>> insert(std::shared_ptr<AVLTreeNode<KeyType>>& tree, KeyType key);
    void insert(KeyType key);//插入一关键字
    std::shared_ptr<AVLTreeNode<KeyType>> search(KeyType key);//搜索一关键字
    inline int height() { return (root) ? root->height : 0; }//二叉树高度
    inline std::shared_ptr<AVLTreeNode<KeyType>> getRoot() { return root; }
private:
    std::shared_ptr<AVLTreeNode<KeyType>> search(std::shared_ptr<AVLTreeNode<KeyType>>& tree, KeyType& key);
    inline int height(std::shared_ptr<AVLTreeNode<KeyType>> node) { return (node) ? node->height : 0; }
    

    /*
     * 四种旋转操作
     */
    std::shared_ptr<AVLTreeNode<KeyType>> leftLeftRotate(std::shared_ptr<AVLTreeNode<KeyType>> node);
    std::shared_ptr<AVLTreeNode<KeyType>> rightRightRotate(std::shared_ptr<AVLTreeNode<KeyType>> node);
    std::shared_ptr<AVLTreeNode<KeyType>> rightLeftRotate(std::shared_ptr<AVLTreeNode<KeyType>> node);
    std::shared_ptr<AVLTreeNode<KeyType>> leftRightRotate(std::shared_ptr<AVLTreeNode<KeyType>> node);
};

template <class KeyType>
std::shared_ptr<AVLTreeNode<KeyType>> AVLTree<KeyType>::insert(std::shared_ptr<AVLTreeNode<KeyType>>& tree, KeyType key)
{
    if (!tree)
    {
        tree = std::make_shared<AVLTreeNode<KeyType>>(key);
    }
    else if (key < tree->key)
    {
        tree->lchild = insert(tree->lchild, key);
        if (height(tree->lchild) - height(tree->rchild) == 2)
        {
            if (key < tree->lchild->key)
            {
                tree = leftLeftRotate(tree);
            }
            else
            {
                tree = leftRightRotate(tree);
            }
        }
    }
    else if (key > tree->key)
    {
        tree->rchild = insert(tree->rchild, key);
        if (height(tree->rchild) - height(tree->lchild) == 2)
        {
            if (key > tree->rchild->key)
            {
                tree = rightRightRotate(tree);
            }
            else
            {
                tree = rightLeftRotate(tree);
            }
        }
    }
    tree->height = max(height(tree->lchild), height(tree->rchild)) + 1;
    return tree;
}

template <class KeyType>
void AVLTree<KeyType>::insert(KeyType key)
{
    insert(root, key);
}

template <class KeyType>
std::shared_ptr<AVLTreeNode<KeyType>> AVLTree<KeyType>::search(KeyType key)
{
    
    if (root)
    {
        if (root->key == key)
            return root;
        else if (key < root->key)
            return search(root->lchild, key);
        else
        {
            return search(root->rchild, key);
        }
    }
    return nullptr;
}

template <class KeyType>
std::shared_ptr<AVLTreeNode<KeyType>> AVLTree<KeyType>::search(std::shared_ptr<AVLTreeNode<KeyType>>& tree, KeyType& key)
{
    if (tree)
    {
        if (tree->key == key)
            return tree;
        else if (key < tree->key)
            return search(tree->lchild, key);
        else
        {
            return search(tree->rchild, key);
        }
    }
    return nullptr;
}

template <class KeyType>
std::shared_ptr<AVLTreeNode<KeyType>> AVLTree<KeyType>::
leftLeftRotate(std::shared_ptr<AVLTreeNode<KeyType>> node)
{
    std::shared_ptr<AVLTreeNode<KeyType>> k;
    k = node->lchild;
    node->lchild = k->rchild;
    k->rchild = node;


    node->height = max(height(node->lchild), height(node->rchild)) + 1;
    k->height = max(height(k->lchild), height(node)) + 1;

    return k;
}

template <class KeyType>
std::shared_ptr<AVLTreeNode<KeyType>> AVLTree<KeyType>::
rightRightRotate(std::shared_ptr<AVLTreeNode<KeyType>> node)
{
    std::shared_ptr<AVLTreeNode<KeyType>> k;
    k = node->rchild;
    node->rchild = k->lchild;
    k->lchild = node;


    node->height = max(height(node->lchild), height(node->rchild)) + 1;
    k->height = max(height(k->rchild), height(node)) + 1;

    return k;
}

template <class KeyType>
std::shared_ptr<AVLTreeNode<KeyType>> AVLTree<KeyType>::
rightLeftRotate(std::shared_ptr<AVLTreeNode<KeyType>> node)
{
    node->rchild = leftLeftRotate(node->rchild);

    return rightRightRotate(node);
}

template <class KeyType>
std::shared_ptr<AVLTreeNode<KeyType>> AVLTree<KeyType>::
leftRightRotate(std::shared_ptr<AVLTreeNode<KeyType>> node)
{
    node->lchild = rightRightRotate(node->lchild);

    return leftLeftRotate(node);
}
