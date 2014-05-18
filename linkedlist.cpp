#include "linkedlist.h"
#include <cstdlib>

LinkedList::LinkedList()
{
    list = (LINKEDLIST*)malloc(sizeof(LINKEDLIST));
    list->head = 0;
    list->tail = 0;
}

void LinkedList::InsertFirst(TREE_NODE* treeNode)
{
    tempNode = NewNode(treeNode);

    if (list->head)
        list->head->prev = tempNode;

    tempNode->prev = 0;
    tempNode->next = list->head;
    list->head = tempNode;

    if (!list->tail)
        list->tail = tempNode;
}

void LinkedList::InsertLast(TREE_NODE* treeNode)
{
    tempNode = NewNode(treeNode);

    if (list->tail)
        list->tail->next = tempNode;

    tempNode->prev = list->tail;
    tempNode->next = 0;
    list->tail = tempNode;

    if (!list->head)
        list->head = tempNode;
}

void LinkedList::RemoveFirst()
{
    if (!list->head)
        return;

    tempNode = list->head;
    list->head = list->head->next;

    if (list->head)
        list->head->prev = 0;

    free(tempNode);

    if (!list->head)
        list->tail = 0;
}

void LinkedList::RemoveLast()
{
    if (!list->tail)
        return;

    tempNode = list->tail;
    list->tail = list->tail->prev;

    if (list->tail)
        list->tail->next = 0;

    free(tempNode);

    if (!list->tail)
        list->head = 0;
}

unsigned int LinkedList::Size()
{
    tempInt = 0;
    tempNode = list->head;
    while(tempNode)
    {
        tempInt++;
        tempNode = tempNode->next;
    }

    return tempInt;
}

TREE_NODE* LinkedList::First()
{
    return list->head?list->head->data:0;
}

TREE_NODE* LinkedList::Last()
{
    return list->tail?list->tail->data:0;
}

LINKEDLIST_NODE* LinkedList::NewNode(TREE_NODE* node)
{
    tempNode = (LINKEDLIST_NODE*)malloc(sizeof(LINKEDLIST_NODE));
    tempNode->next = tempNode->prev = 0;
    tempNode -> data = node;
    return tempNode;
}

bool LinkedList::Empty()
{
    return !list->head;
}

void LinkedList::Free()
{
    while(!Empty())
        RemoveLast();
}

LinkedList::~LinkedList()
{
    Free();
    free(list);
}
