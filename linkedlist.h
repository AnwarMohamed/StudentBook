#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "studenttree.h"

struct LINKEDLIST_NODE
{
    TREE_NODE* data;
    struct LINKEDLIST_NODE* next, *prev;
};

struct LINKEDLIST
{
    LINKEDLIST_NODE* head, *tail;
};

class LinkedList
{
    LINKEDLIST* list;
    LINKEDLIST_NODE* NewNode(TREE_NODE* node);

    LINKEDLIST_NODE* tempNode;
    unsigned int tempInt;
public:
    LinkedList();
    ~LinkedList();

    void    InsertFirst(TREE_NODE* node);
    void    InsertLast(TREE_NODE* node);
    void    RemoveFirst();
    void    RemoveLast();
    void    Free();
    bool    Empty();
    unsigned int Size();
    TREE_NODE* First();
    TREE_NODE* Last();
};

#endif // LINKEDLIST_H
