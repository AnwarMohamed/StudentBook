#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QLineEdit>
#include <QRadioButton>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setupEnvironment();
    ui->setupUi(this);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    setupMenubar();
    setupStatusbar();
    setupToolbar();
    setupTableRecords();

    resizeEvent(0);
}

void MainWindow::setupTableRecords()
{
    table = ui->tableView;
    table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    tableModel = new StudentModel();

    //tableModel->SetMode(sortMode | orderMode);
    table->setModel(tableModel);
    //table->setSpan(0, 1, 1, 2);
    //table->resizeColumnsToContents();
}

void MainWindow::checkEnability(bool active)
{
    if (!active)
    {
        ui->toolBar->setEnabled(false);
        ui->menuRecords->setEnabled(false);
    }
    else if (tableModel->Size() > 0)
    {
        ui->toolBar->setEnabled(true);
        ui->menuRecords->setEnabled(true);

        for (int i=1; i<ui->toolBar->actions().size(); i++)
            ui->toolBar->actions()[i]->setEnabled(true);

        for (int i=1; i<ui->menuRecords->actions().size(); i++)
            ui->menuRecords->actions()[i]->setEnabled(true);
    }
    else
    {
        ui->toolBar->setEnabled(true);
        ui->menuRecords->setEnabled(true);

        for (int i=1; i<ui->toolBar->actions().size(); i++)
            ui->toolBar->actions()[i]->setEnabled(false);

        for (int i=1; i<ui->menuRecords->actions().size(); i++)
            ui->menuRecords->actions()[i]->setEnabled(false);
    }
}

void MainWindow::setupMenubar()
{
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

    connect(ui->actionCreators ,SIGNAL(triggered(bool)), this, SLOT(aboutDialog()));
    connect(ui->actionEdit ,SIGNAL(triggered(bool)), this, SLOT(editMenu()));
    connect(ui->actionDelete ,SIGNAL(triggered(bool)), this, SLOT(deleteMenu()));
}

void MainWindow::deleteMenu()
{
    indexList = table->selectionModel()->selectedIndexes();
    if (!indexList.isEmpty())
        tableModel->removeRow(indexList.value(0).row());

    checkEnability(true);
}

void MainWindow::editMenu()
{
    indexList = table->selectionModel()->selectedIndexes();
    if (!indexList.isEmpty())
        table->edit(indexList.value(0));
}

void MainWindow::setupEnvironment()
{
    tableModel = 0;
    dataSaved = true;
    sortMode = SORT_BY_ID;
    orderMode = VIEW_ORDER_PRE;
}

void MainWindow::changeViewSort(int sortType)
{
    sortMode = sortType;

    if (tableModel)
        tableModel->SetMode(sortMode | orderMode);
}

void MainWindow::changeViewOrder(int orderType)
{
    orderMode = orderType;

    if (tableModel)
        tableModel->SetMode(sortMode | orderMode);
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

void MainWindow::setupToolbar()
{
    toolbar = ui->toolBar;
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);

    QToolButton* button = new QToolButton;
    button->setIcon(QIcon(":/images/database-add-icon.png"));
    button->setToolTip("Add Record");
    button->setText(button->toolTip());
    button->setStatusTip(button->toolTip());
    toolbar->addWidget(button);

    button = new QToolButton;
    button->setIcon(QIcon(":/images/database-remove-icon.png"));
    button->setToolTip("Delete Record");
    button->setText(button->toolTip());
    button->setStatusTip(button->toolTip());

    connect(button ,SIGNAL(clicked()), this, SLOT(deleteMenu()));
    toolbar->addWidget(button);

    toolbar->addSeparator();

    QLineEdit* editline = new QLineEdit;
    editline->setStatusTip("Search Record");
    editline->setClearButtonEnabled(true);
    toolbar->addWidget(editline);

    QRadioButton* radiobutton = new QRadioButton;
    radiobutton->setText("By ID");
    radiobutton->setStatusTip("Search By ID");
    radiobutton->setChecked(true);
    toolbar->addWidget(radiobutton);

    radiobutton = new QRadioButton;
    radiobutton->setText("By Name");
    radiobutton->setStatusTip("Search By Name");
    toolbar->addWidget(radiobutton);

    button = new QToolButton;
    button->setIcon(QIcon(":/images/database-search-icon.png"));
    button->setToolTip("Search Record");
    button->setText(button->toolTip());
    button->setStatusTip(button->toolTip());
    toolbar->addWidget(button);

}

void MainWindow::setupStatusbar()
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
