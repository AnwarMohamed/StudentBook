#include "adddialog.h"
#include "ui_adddialog.h"
#include <QDesktopWidget>
#include "mainwindow.h"
#include <QLayout>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);

    QWidget::setFixedSize(width(), height());

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());
    setWindowTitle("Add A New Student");

    connect(ui->pushAdd,SIGNAL(clicked()),this,SLOT(insertadd()) );
}

AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::SetModel(StudentModel *model)
{
    this->model=model;
}

void AddDialog::insertadd()
{
    model->Insert(
                ui->editId->text().toUInt(),
                ui->editFullname->text().toLocal8Bit().data(),
                0,
                true);
    close();
}



