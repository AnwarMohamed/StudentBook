#include "studentmodel.h"
#include <cstdlib>

StudentModel::StudentModel(): QAbstractTableModel()
{
    dataTree = new StudentTree;
}

int StudentModel::rowCount(const QModelIndex& parent) const
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

    if(index.row() >= dataTree->Size() || index.row() < 0)
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
    /*if (index.isValid() && role == Qt::EditRole && !(index.row() >= mydata.size() || index.row() < 0))
    {
        int row = index.row();

        switch(index.column())
        {
        case 0:
            mydata[row].setFirstname(value.toString());
            break;
        case 1:
            mydata[row].setLastname(value.toString());
            break;
        case 2:
            mydata[row].setEmail(value.toString());
            break;
        default:
            return false;
        }
        emit dataChanged(index, index);
        return true;
    }*/
    return false;
}

Qt::ItemFlags StudentModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
