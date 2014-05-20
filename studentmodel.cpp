#include "studentmodel.h"
#include <cstdlib>

StudentModel::StudentModel(): QAbstractTableModel()
{
    dataTree = new StudentTree(this);

    for (int i=0; i<10; i++)
        dataTree->Insert(i, "Anwar", 0);

    //for (int i=50; i<100; i++)
    //    dataTree->Insert(i, "Anwar");

    //for (int i=10; i<40; i++)
    //    dataTree->Insert(i, "Anwar");

    dataTree->SetMode(SORT_BY_ID | VIEW_ORDER_IN);
}

void StudentModel::SetMode(int mode)
{
    dataTree->SetMode(mode);
}

unsigned int StudentModel::Size()
{
    return dataTree->Size();
}

int StudentModel::rowCount(const QModelIndex&) const
{
    return dataTree->Size();
}

int StudentModel::columnCount(const QModelIndex& parent) const
{
    return parent.isValid()? 0:2;
}

QVariant StudentModel::data(const QModelIndex& index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if((unsigned int)index.row() >= dataTree->Size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch(index.column())
        {
        case ID:
            return QString::number(dataTree->Get(index.row())->data->id);
        case FULLNAME:
            return tr(dataTree->Get(index.row())->data->fullname);
        }
    }
    return QVariant();
}

QVariant StudentModel::headerData(int section, Qt::Orientation orientation, int role) const
{

    if(role == Qt::DisplayRole)
    {
        if(orientation == Qt::Horizontal)
        {
            switch (section) {
            case 0:
                return tr(HEADER_COLUMN_0);
            case 1:
                return tr(HEADER_COLUMN_1);
            }
        }
        else if(orientation == Qt::Vertical)
            return QString::number(section + 1);
    }

    return QVariant::Invalid;
}


bool StudentModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole && !((unsigned int)index.row() >= dataTree->Size() || index.row() < 0))
    {
        int row = index.row();

        switch(index.column())
        {
        case ID:
            if (!dataTree->Set(row, value.toInt()))
                return false;
            break;
        case FULLNAME:
            if (!dataTree->Set(row, value.toString().toLocal8Bit().data()))
                return false;
            break;
        default:
            return false;
        }
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags StudentModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool StudentModel::removeRow(int row, const QModelIndex&)
{

    beginRemoveRows(QModelIndex(),row,row);
    dataTree->Delete(row);
    endRemoveRows();
    return true;
}

StudentModel::~StudentModel()
{

}
