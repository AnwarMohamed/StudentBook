#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    createStatusBar();
    initVariables();

    connect(ui->actionCreators ,SIGNAL(triggered(bool)), this, SLOT(aboutDialog()));

    resizeEvent(0);
}

void MainWindow::initVariables()
{
    dataSaved = true;
    table = ui->tableView;

    table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    orderGroup = new QActionGroup(this);
    ui->actionInOrder->setActionGroup(orderGroup);
    ui->actionPreOrder->setActionGroup(orderGroup);
    ui->actionPostOrder->setActionGroup(orderGroup);

    orderSignalMapper = new QSignalMapper (this) ;
    connect(ui->actionInOrder, SIGNAL(changed()), orderSignalMapper, SLOT(map()));
    connect(ui->actionPreOrder, SIGNAL(changed()), orderSignalMapper, SLOT(map()));
    connect(ui->actionPostOrder, SIGNAL(changed()), orderSignalMapper, SLOT(map()));

    orderSignalMapper->setMapping(ui->actionPreOrder, VIEW_ORDER_PRE);
    orderSignalMapper->setMapping(ui->actionPostOrder, VIEW_ORDER_POST);
    orderSignalMapper->setMapping(ui->actionInOrder, VIEW_ORDER_IN);

    connect (orderSignalMapper, SIGNAL(mapped(int)), this, SLOT(changeViewOrder(int))) ;
    ui->actionPreOrder->setChecked(true);


    sortGroup = new QActionGroup(this);
    ui->actionBy_ID->setActionGroup(sortGroup);
    ui->actionBy_Name->setActionGroup(sortGroup);

    sortSignalMapper = new QSignalMapper (this) ;
    connect(ui->actionBy_ID, SIGNAL(changed()), sortSignalMapper, SLOT(map()));
    connect(ui->actionBy_Name, SIGNAL(changed()), sortSignalMapper, SLOT(map()));

    sortSignalMapper->setMapping(ui->actionBy_ID, SORT_BY_ID);
    sortSignalMapper->setMapping(ui->actionBy_Name, SORT_BY_NAME);

    connect (sortSignalMapper, SIGNAL(mapped(int)), this, SLOT(changeViewSort(int))) ;
    ui->actionBy_ID->setChecked(true);
}


void MainWindow::changeViewSort(int sortType)
{

}

void MainWindow::changeViewOrder(int orderType)
{
    //printf("triggered");
}

void MainWindow::resizeEvent(QResizeEvent * event)
{
    ui->tableView->resize(
                geometry().width() - 20,
                geometry().height() - 110
                );

}

void MainWindow::aboutDialog()
{
    AboutDialog about;
    about.show();
    about.exec();
}

bool MainWindow::checkSaveMessage()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setWindowModality(Qt::NonModal);
    msgBox->setWindowTitle("Save Changes ?");
    msgBox->setText("Do you want to save your changes?");
    msgBox->setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                               QMessageBox::Cancel);
    msgBox->setDefaultButton(QMessageBox::Save);
    int ret = msgBox->exec();

    if (ret == QMessageBox::Cancel)
        return false;
    else
        return true;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!dataSaved)
    {
        if (checkSaveMessage())
            event->accept();
        else
            event->ignore();
    }
}

void MainWindow::createStatusBar()
{
    QLabel* statusLabel = new QLabel(" Welcome to Student Book Pro ");
    statusLabel->setAlignment(Qt::AlignLeft);
    statusBar()->addWidget(statusLabel);

    QLabel* namesLabel = new QLabel(" Abdallah Elerian & Anwar Mohamed ");
    namesLabel->setAlignment(Qt::AlignRight);
    statusBar()->addPermanentWidget(namesLabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
