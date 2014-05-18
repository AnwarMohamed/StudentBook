#ifndef STUDENTTREE_H
#define STUDENTTREE_H

struct TREE_NODE_DATA
{
    unsigned int id;
    char *fullname;
};

struct TREE_NODE
{
    TREE_NODE_DATA *data;
    TREE_NODE *right;
    TREE_NODE * left;
};

struct TREE
{
    TREE_NODE *root;
};

#define BST_MODE_ID     1
#define BST_MODE_NAME   2

class StudentTree
{

private:
    TREE *bstId, *bstName;
    TREE_NODE* tempNode;
    TREE_NODE_DATA* tempData;
    unsigned int treeSize;

    void    InsertSortedId(TREE_NODE** root, TREE_NODE* node);
    TREE_NODE*   NewNode(TREE_NODE_DATA* data);
    void    DeleteNode(TREE_NODE** node);
    TREE_NODE**  SmallestNode(TREE_NODE** node);

    void    InitIterator();
    struct
    {
        int index;
        TREE_NODE* node;
    } iterator;


public:
    StudentTree();
    ~StudentTree();

    int     Size();
    TREE_NODE*   Get(int index);
    void    Insert(unsigned int id, char* fullname);


    TREE_NODE*   IteratorBegin();
    int     IteratorCurrentIndex();
    TREE_NODE*   IteratorCurrent();
    TREE_NODE*   IteratorInc();
    bool    IteratorEnd();
    TREE_NODE*   IteratorGoTo(int index);

    void    Print(TREE_NODE* root);
};

#endif // STUDENTTREE_H
