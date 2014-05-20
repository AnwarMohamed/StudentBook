#ifndef STUDENTTREE_H
#define STUDENTTREE_H

#define BST_MODE_ID     1
#define BST_MODE_NAME   2

#include "structs.h"
#include "linkedlist.h"
#include <QAbstractTableModel>


#define VIEW_ORDER_PRE  1
#define VIEW_ORDER_IN   2
#define VIEW_ORDER_POST 4
#define SORT_BY_ID      8
#define SORT_BY_NAME    16

#define DATA_GLOCAL_ALLOC   1
#define DATA_USER_ALLOC     2
#define DATA_NO_ALLOC       3

class StudentTree
{

private:
    TREE *bstId, *bstName, *viewTree;
    TREE_NODE*      tempNode;
    TREE_NODE_DATA* tempData;
    unsigned int    tempUint;
    unsigned int    treeSize;
    LinkedList*     viewList;
    QAbstractTableModel* model;

    void        InsertSortedId(TREE_NODE** root, TREE_NODE* node);
    TREE_NODE*  NewNode(TREE_NODE_DATA* data);
    void        DeleteNode(TREE_NODE** node);
    TREE_NODE** SmallestNode(TREE_NODE** node);
    void        GeneratePreOrder(TREE_NODE* root);
    void        GenerateInOrder(TREE_NODE* root);
    void        GeneratePostOrder(TREE_NODE* root);
public:
    StudentTree(QAbstractTableModel* model);
    ~StudentTree();

    unsigned int Size();
    TREE_NODE*  Get(unsigned int index);
    void        Insert(unsigned int id, char* fullname, unsigned int flags);
    void        SetMode(int mode);

    void        Delete(unsigned int index);
    void        Delete(TREE_NODE* node, unsigned int flags);

    TREE_NODE** Search(TREE_NODE* node);
    LinkedList* Search(char* fullname);
    TREE_NODE** Search(unsigned int id);

    bool        Set(unsigned int index, unsigned int id);
    bool        Set(unsigned int index, char* fullname);

    bool        Exists(unsigned int id);
};

#endif // STUDENTTREE_H
