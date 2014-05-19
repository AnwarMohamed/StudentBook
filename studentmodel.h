#ifndef STUDENTMODEL_H
#define STUDENTMODEL_H

#include <QAbstractTableModel>
#include "studenttree.h"

#define HEADER_COLUMN_0 "ID"
#define HEADER_COLUMN_1 "Full Name"

class StudentModel: public QAbstractTableModel
{
    Q_OBJECT

    StudentTree* dataTree;

public:
    StudentModel();
    ~StudentModel();

    enum {ID=0,FULLNAME};

    int rowCount(const QModelIndex& parent) const;
    int columnCount(const QModelIndex& parent) const;
    QVariant data(const QModelIndex& index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool removeRow(int row, const QModelIndex & parent = QModelIndex());

    void SetMode(int mode);
};

#endif // STUDENTMODEL_H
