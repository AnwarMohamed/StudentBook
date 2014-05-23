#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QMouseEvent>
#include "studentmodel.h"

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    void SetModel(StudentModel *model);
    ~AddDialog();

public slots:
    void insertadd();

private:
    Ui::AddDialog *ui;
    StudentModel *model;



};
#endif // ADDDIALOG_H
