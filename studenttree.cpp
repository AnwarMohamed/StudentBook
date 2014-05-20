#include "studenttree.h"
#include <cstdlib>
#include <cstdio>
#include "misc.h"

StudentTree::StudentTree(QAbstractTableModel* model)
{
    bstId = (TREE*)malloc(sizeof(TREE));
    bstId->root = 0;

    bstName = (TREE*)malloc(sizeof(TREE));
    bstName->root = 0;

    treeSize = 0;

    viewList = new LinkedList();
    this->model = model;

    treeMode = SORT_BY_ID | VIEW_ORDER_PRE;
}

bool StudentTree::Exists(unsigned int id)
{
    viewList->IteratorReset();

    if (viewList->IteratorCurrent() &&
            viewList->IteratorCurrent()->data->id == id)
        return true;

    while((tempNode = viewList->IteratorInc()))
        if (tempNode->data->id == id)
            return true;
    return false;
}

bool StudentTree::Set(unsigned int index, unsigned int id)
{
    if (Exists(id))
        return false;

    tempNode = viewList->IteratorGoTo(index);
    if (tempNode)
    {
        tempNode->data->id = id;
        return true;
    }

    return false;
}

bool StudentTree::Set(unsigned int index, char* fullname)
{
    tempNode = viewList->IteratorGoTo(index);
    if (tempNode)
    {
        if(!ValidFullname(fullname, &tempUint))
            return false;

        if (tempNode->data->flags & DATA_USER_ALLOC)
            tempNode->data->fullname = (char*)realloc(tempNode->data->fullname, tempUint + 1);
        else
            tempNode->data->fullname = (char*)malloc(tempUint + 1);

        strcpy(tempNode->data->fullname, fullname);

        tempNode->data->flags ^= (DATA_GLOCAL_ALLOC | DATA_NO_ALLOC);
        tempNode->data->flags |= DATA_USER_ALLOC;

        return true;
    }

    return false;
}

bool StudentTree::ValidFullname(char* fullname, unsigned int* len)
{
    tempUint = strlen(fullname);
    remove_comma(fullname, 0, tempUint);
    trim(fullname);
    f_capital(fullname);
    tempUint = strlen(fullname);

    return tempUint?(len?(*len = tempUint):true):false;
}

void StudentTree::Delete(unsigned int index)
{
    //TREE_NODE* node = Get(index);
    //TREE_NODE** pNode = Search(bstId, node);
    viewList->RemoveFirst();

}

//TREE_NODE** Search(TREE* tree, TREE_NODE* node)
//{

//}

unsigned int StudentTree::Size()
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

void StudentTree::Insert(unsigned int id, char* fullname, unsigned int flags, bool reorder)
{
    tempData = (TREE_NODE_DATA*)malloc(sizeof(TREE_NODE_DATA));
    tempData->id = id;
    tempData->fullname = fullname;
    tempData->flags = flags;

    tempNode = NewNode(tempData);

    InsertSortedId(&bstId->root, tempNode);
    //InsertSortedName(&bstName->root, tempNode);

    treeSize++;

    if (reorder)
        SetMode(treeMode);
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

    treeMode = mode;
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
