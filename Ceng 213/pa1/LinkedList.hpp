#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>

#include "Node.hpp"

template <class T>
class LinkedList
{
    public: // DO NOT CHANGE THIS PART.
        LinkedList();
        LinkedList(const LinkedList &obj);

        ~LinkedList();

        Node<T> *getFirstNode() const; //
        Node<T> *getHead() const;
        Node<T> *getTail() const;
        int getNumberOfNodes();
        bool isEmpty(); //

        void insertAtTheFront(const T &data); //
        void insertAtTheEnd(const T &data); //
        void insertAfterGivenNode(const T &data, Node<T> *prev); //

        void removeNode(Node<T> *node);
        void removeAllNodes();

        Node<T> *findNode(const T &data); //

        void printAllNodes();
        void printReversed();

        LinkedList &operator=(const LinkedList &rhs);

    private: // YOU MAY ADD YOUR OWN UTILITY MEMBER FUNCTIONS HERE.

    private: // DO NOT CHANGE THIS PART.
        Node<T> *head;
        Node<T> *tail;
};

template<class T>
LinkedList<T>::LinkedList()
{
    /* TODO */
    head = new Node<T>;
    tail = new Node<T>;

    head->next = tail;
    tail->prev = head;

}

template<class T>
LinkedList<T>::LinkedList(const LinkedList &obj)
{
    /* TODO */
    Node<T> *tmp_node = obj.head;
    head = new Node<T>(*tmp_node);
    Node<T> *tmp_new = head;

    while( tmp_node != obj.tail )
    {
      tmp_node = tmp_node->next;
      tmp_new->next = new Node<T>(*tmp_node);
      tmp_new->next->prev = tmp_new;
      tmp_new = tmp_new->next;
    }

    tmp_new->next = NULL;
    tail = tmp_new;
}

template<class T>
LinkedList<T>::~LinkedList()
{
    /* TODO */
    head = head->next;
    while( head != tail )
    {
      delete head->prev;
      head = head->next;
    }
    delete head->prev;
    delete head;
}

template<class T>
Node<T> * LinkedList<T>::getFirstNode() const
{
    /* TODO */
    if( head->next == tail )
      return NULL;
    return head->next;
}

template<class T>
Node<T> * LinkedList<T>::getHead() const
{
    /* TODO */
    return head;
}

template<class T>
Node<T> * LinkedList<T>::getTail() const
{
    /* TODO */
    return tail;
}

template<class T>
int LinkedList<T>::getNumberOfNodes()
{
    /* TODO */
    int num = 0;
    for( Node<T> *tmp = head->next ; tmp != tail ; tmp = tmp->next )
      ++num;
    return num;
}

template<class T>
bool LinkedList<T>::isEmpty()
{
    /* TODO */
    if( head->next == tail )
      return true;
    return false;
}

template<class T>
void LinkedList<T>::insertAtTheFront(const T &data)
{
    /* TODO */
    Node<T> *tmp = new Node<T>(data, head, head->next);
    head->next = tmp;
    tmp->next->prev = tmp;
}

template<class T>
void LinkedList<T>::insertAtTheEnd(const T &data)
{
    /* TODO */
    Node<T> *tmp = new Node<T>(data, tail->prev, tail);
    tail->prev = tmp;
    tmp->prev->next = tmp;
}

template<class T>
void LinkedList<T>::insertAfterGivenNode(const T &data, Node<T> *prev)
{
    /* TODO */
    Node<T> *tmp = new Node<T>(data, prev, prev->next);
    prev->next = tmp;
    tmp->next->prev = tmp;
}

template<class T>
void LinkedList<T>::removeNode(Node<T> *node)
{
    /* TODO */
    for( Node<T> *tmp = head->next ; tmp != tail ; tmp = tmp->next )
      if( tmp == node )
      {
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        delete tmp;
        return ;
      }
}

template<class T>
void LinkedList<T>::removeAllNodes()
{
    /* TODO */
    Node<T> *tmp = head->next;
    while(tmp != tail)
    {
      tmp = tmp->next;
      delete tmp->prev;
    }
    head->next = tail;
    tail->prev = head;
}

template<class T>
Node<T> * LinkedList<T>::findNode(const T &data)
{
    /* TODO */
    for( Node<T> *tmp = head->next ; tmp != tail ; tmp = tmp->next )
      if( tmp->element == data )
        return tmp;
    return NULL;
}

template<class T>
void LinkedList<T>::printAllNodes()
{
    /* TODO */
    for( Node<T> *tmp = head->next ; tmp != tail ; tmp = tmp->next )
      std::cout<<tmp->element<<'\n';
}

template<class T>
void LinkedList<T>::printReversed()
{
    /* TODO */
    for( Node<T> *tmp = tail->prev ; tmp != head ; tmp = tmp->prev )
      std::cout<<tmp->element<<'\n';
}

template<class T>
LinkedList<T> & LinkedList<T>::operator=(const LinkedList &rhs)
{
    /* TODO */
    removeAllNodes();
    delete head;
    delete tail;
    LinkedList<T> *tmp = new LinkedList<T>(rhs);
    head = tmp->head;
    tail = tmp->tail;
    return *this;
}

#endif //LINKED_LIST_HPP
