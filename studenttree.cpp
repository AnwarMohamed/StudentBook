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
    tempLList = new LinkedList();
    this->model = model;

    treeMode = SORT_BY_ID | VIEW_ORDER_PRE;
}

TREE_NODE** StudentTree::Search(unsigned int id)
{
    viewList->Free();
    pTempNode = Search(&(bstId->root), id);
    if (pTempNode)
        viewList->InsertLast(*pTempNode );
}

TREE_NODE** StudentTree::Search(TREE_NODE** root, unsigned int Id)
{
    if(!root || !*root)
        return 0;

    else if((*root)->data->id == Id)
        return root;

    else if ((*root)->data->id < Id)
        return Search(&((*root)->right),Id);

    else
        return Search(&((*root)->left),Id);
}

bool StudentTree::LoadFromFile(char* filename)
{

    //UnloadFile();

    fileBufferSize =0;


    //tempUint = get_file_size (filename);
    fileBuffer = (char*)malloc (tempUint * sizeof(char));

    if (!fileBuffer)
        return false;

    fileObj = fopen(filename, "rw");
    if (!fileObj)
        return false;

    fileBufferSize = fread(fileBuffer, sizeof (char), tempUint, fileObj);

    if (fileBufferSize != tempUint)
        return false;

    tempUint = fclose(fileObj);
    if (tempUint)
        return false;


}


bool StudentTree::SaveToFile(char* filename)
{

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

void StudentTree::SearchSub(char* fullname)
{
    tempLList->Free();

    for(tempUint=0; tempUint<viewList->Size(); tempUint++)
    {
        tempNode = viewList->IteratorGoTo(tempUint);
        if(sub_string(tempNode->data->fullname, fullname))
            tempLList->InsertLast(tempNode);
    }

    viewList->Free();
    for(tempUint=0; tempUint<tempLList->Size(); tempUint++)
        viewList->InsertLast(tempLList->IteratorGoTo(tempUint));

    tempLList->Free();
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
    InsertSortedId(&(bstId->root), tempNode);

    tempNode = NewNode(tempData);
    InsertSortedName(&(bstName->root), tempNode);

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

void StudentTree::InsertSortedName(TREE_NODE** root, TREE_NODE* node)
{
    if (!*root)
        *root = node;

    else if (strcmp((*root)->data->fullname, node->data->fullname) < 0)
    {
        if (!(*root)->right)
            (*root)->right = node;
        else
            InsertSortedName(&((*root)->right), node);
    }
    else
    {
        if (!(*root)->left)
            (*root)->left = node;
        else
            InsertSortedName(&((*root)->left), node);
    }
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
    treeMode = mode;

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

void StudentTree::GeneratePreOrder(TREE_NODE* root, bool reflect, LinkedList* list)
{
    if (!root) return;

    if (reflect)
        viewList->InsertLast(root);
    else if (!reflect && list)
        list->InsertLast(root);

    GeneratePreOrder(root->left);
    GeneratePreOrder(root->right);
}

void StudentTree::GenerateInOrder(TREE_NODE* root, bool reflect, LinkedList* list)
{
    if (!root) return;
    GeneratePreOrder(root->left);

    if (reflect)
        viewList->InsertLast(root);
    else if (!reflect && list)
        list->InsertLast(root);

    GeneratePreOrder(root->right);
}

void StudentTree::GeneratePostOrder(TREE_NODE* root, bool reflect, LinkedList* list)
{
    if (!root) return;
    GeneratePreOrder(root->left);
    GeneratePreOrder(root->right);

    if (reflect)
        viewList->InsertLast(root);
    else if (!reflect && list)
        list->InsertLast(root);
}
