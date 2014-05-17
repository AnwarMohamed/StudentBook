#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QDesktopWidget>
#include <QTimer>
#include "mainwindow.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowFlags(Qt::FramelessWindowHint);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    setWindowTitle("About Us");
}

void AboutDialog::setAsSplash(int ms)
{
    setWindowTitle("Student Book Pro");
    QTimer::singleShot(ms, this, SLOT(accept()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::mouseReleaseEvent(QMouseEvent*)
{
    close();
}
