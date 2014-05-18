#include "studenttree.h"
#include <cstdlib>
#include <cstdio>

StudentTree::StudentTree()
{
    bstId = (TREE*)malloc(sizeof(TREE));
    bstId->root = 0;

    bstName = (TREE*)malloc(sizeof(TREE));
    bstName->root = 0;

    treeSize = 0;

    viewList = new LinkedList;
}

int StudentTree::Size()
{
    return viewList->Size();
}

TREE_NODE* StudentTree::Get(unsigned int index)
{
    return viewList->IteratorGoTo(index);
}

TREE_NODE * StudentTree::NewNode(TREE_NODE_DATA *data)
{
    tempNode =(TREE_NODE*)malloc(sizeof(TREE_NODE));
    tempNode->data = data;
    tempNode->left = tempNode->right = 0;
    return tempNode;
}

void StudentTree::Insert(unsigned int id, char* fullname)
{
    tempData = (TREE_NODE_DATA*)malloc(sizeof(TREE_NODE_DATA));
    tempData->id = id;
    tempData->fullname = fullname;

    tempNode = NewNode(tempData);

    InsertSortedId(&bstId->root, tempNode);
    //InsertSortedName(&bstName->root, tempNode);

    treeSize++;
}

TREE_NODE** StudentTree::SmallestNode(TREE_NODE** node)
{
    if (!(*node)) return NULL;
    if (!(*node)->left) return node;
    else return SmallestNode(&(*node)->left);
}

void StudentTree::DeleteNode(TREE_NODE** node)
{
    if (!node || !(*node)) return;
    if (!(*node)->left && !(*node)->right)
    {
        free(*node);
        *node = 0;
    }
    else if ((*node)->left && (*node)->right)
    {
        TREE_NODE** ppNode = SmallestNode(&(*node)->right);
        (*node)->data = (*ppNode)->data;
        free(*ppNode);
        *ppNode = 0;
    }
    else
    {
        tempNode = (*node)->left?(*node)->left:(*node)->right;
        free(*node);
        *node = tempNode;
    }

    treeSize--;
}

void StudentTree::Print(TREE_NODE* root)
{
    if (!root) return;


    Print(root->left);
    printf("%d ", root->data->id);
    Print(root->right);

}

void StudentTree::InsertSortedId(TREE_NODE** root, TREE_NODE* node)
{
    if (!*root)
        *root = node;

    else if ((*root)->data->id <= node->data->id)
    {
        if (!(*root)->right)
            (*root)->right = node;
        else
            InsertSortedId(&((*root)->right), node);
    }
    else
    {
        if (!(*root)->left)
            (*root)->left = node;
        else
            InsertSortedId(&((*root)->left), node);
    }
}

StudentTree::~StudentTree()
{
    free(bstId);
    free(bstName);
    delete viewList;
}

void StudentTree::SetMode(int mode)
{
    if (mode & SORT_BY_ID)
        viewTree = bstId;
    else if (mode & SORT_BY_NAME)
        viewTree = bstName;

    viewList->Free();

    if (mode & VIEW_ORDER_PRE)
        GeneratePreOrder(viewTree->root);
    else if (mode & VIEW_ORDER_IN)
        GenerateInOrder(viewTree->root);
    else if (mode & VIEW_ORDER_POST)
        GeneratePostOrder(viewTree->root);

}

void StudentTree::GeneratePreOrder(TREE_NODE* root)
{
    if (!root) return;
    viewList->InsertLast(root);
    GeneratePreOrder(root->left);
    GeneratePreOrder(root->right);
}

void StudentTree::GenerateInOrder(TREE_NODE* root)
{
    if (!root) return;
    GeneratePreOrder(root->left);
    viewList->InsertLast(root);
    GeneratePreOrder(root->right);
}

void StudentTree::GeneratePostOrder(TREE_NODE* root)
{
    if (!root) return;
    GeneratePreOrder(root->left);
    GeneratePreOrder(root->right);
    viewList->InsertLast(root);
}
