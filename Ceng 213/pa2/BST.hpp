#ifndef BST_HPP
#define BST_HPP

#include <iostream>

#include "BSTNode.hpp"

template <class T>
class BST {
public: // DO NOT CHANGE THIS PART.
    BST();
    BST(const BST<T> &obj);

    ~BST();

    BSTNode<T> *getRoot();

    void insert(const T &data);
    void remove(const T &data);
    BSTNode<T> *find(const T &data);
    void printInorder();

    BST<T> &operator=(const BST<T> &rhs);

private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.
    void printInorder(BSTNode<T> *node);

    void deleteAll(BSTNode<T> * &node);
    BSTNode<T> *constructAll(BSTNode<T> * const &node);
    void removeNode(BSTNode<T> * &node, const T &data);

private: // DO NOT CHANGE THIS PART.
    BSTNode<T> *root;
};

#endif //BST_HPP

template <class T>
BST<T>::BST() {
    /* TODO */
    root = NULL;
}

template <class T>
BST<T>::BST(const BST<T> &obj) {
    /* TODO */
    root = constructAll(obj.root);
}

template <class T>
BST<T>::~BST() {
    /* TODO */
    deleteAll(root);
}

template <class T>
BSTNode<T> *BST<T>::getRoot() {
    /* TODO */
    return root;
}

template <class T>
void BST<T>::insert(const T &data) {
    /* TODO */
    if(root == NULL)
    {
      root = new BSTNode<T>(data, NULL, NULL);
      return;
    }
    BSTNode<T> *tmp = root;

    while(true)
    {
      if(data < tmp->data)
      {
        if( !(tmp->left) )
        {
          tmp->left = new BSTNode<T>(data, NULL, NULL);
          return;
        }
        tmp = tmp->left;
      }
      else
      {
        if( !(tmp->right) )
        {
          tmp->right = new BSTNode<T>(data, NULL, NULL);
          return;
        }
        tmp = tmp->right;
      }
    }
}

template <class T>
void BST<T>::remove(const T &data) {
    /* TODO */
    removeNode(root, data);
}

template <class T>
BSTNode<T> *BST<T>::find(const T &data) {
    /* TODO */
    BSTNode<T> *tmp = root;

    while(tmp)
    {
      if(data < tmp->data)
        tmp = tmp->left;
      else if(data > tmp->data)
        tmp = tmp->right;
      else
        return tmp;
    }
    return NULL;
}

template <class T>
void BST<T>::printInorder() {

    if (root == NULL) {
        std::cout << "{}" << std::endl;
    } else {
        std::cout << "{" << std::endl;
        printInorder(root);
        std::cout << std::endl << "}" << std::endl;
    }
}

template <class T>
void BST<T>::printInorder(BSTNode<T> *node) {

    if (node == NULL)
        return;

    printInorder(node->left);
    if (node->left) {
        std::cout << "," << std::endl;
    }
    std::cout << "\t" << node->data;
    if (node->right) {
        std::cout << "," << std::endl;
    }
    printInorder(node->right);
}

template <class T>
BST<T> &BST<T>::operator=(const BST<T> &rhs) {
    /* TODO */
    deleteAll(root);
    root = constructAll(rhs.root);
    return *this;
}

template <class T>
void BST<T>::deleteAll(BSTNode<T> * &node) {

  if(node == NULL)
    return;

  if(node->left)
    deleteAll(node->left);

  if(node->right)
    deleteAll(node->right);

  delete node;
  node = NULL;

}

template <class T>
BSTNode<T> *BST<T>::constructAll(BSTNode<T> * const &node) {

  if(node == NULL)
    return NULL;

  BSTNode<T> *tmp = new BSTNode<T>(node->data, NULL, NULL);

  if(node->right)
    tmp->right = constructAll(node->right);

  if(node->left)
    tmp->left = constructAll(node->left);

  return tmp;
}

template <class T>
void BST<T>::removeNode(BSTNode<T> * &node, const T &data) {

  if(node == NULL)
    return;
  if(data < node->data)
    removeNode(node->left, data);
  else if(data > node->data)
    removeNode(node->right, data);
  else if(node->left && node->right)
  {
    BSTNode<T> *tmp = node->right;
    while(tmp->left)
      tmp = tmp->left;
    node->data = tmp->data;
    removeNode(node->right, node->data);
  }
  else
  {
    BSTNode<T> *tmp = node;
    node = (node->left) ? node->left : node->right;
    delete tmp;
  }
}
