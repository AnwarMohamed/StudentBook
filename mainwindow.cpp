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
    tableModel->SetMode(sortMode | orderMode);

    table->setModel(tableModel);
    table->horizontalHeader()->setStretchLastSection(true);
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
    connect(ui->actionAdd_New ,SIGNAL(triggered(bool)), this, SLOT(addMenu()));
}

void MainWindow::deleteMenu()
{
    indexList = table->selectionModel()->selectedIndexes();
        tableModel->removeRow(indexList.value(0).row());
        if (!indexList.isEmpty())

    checkEnability(true);
}

void MainWindow::addMenu()
{
    tableModel->insertRow(0);
    tableModel->insertColumn(0);
    tableModel->insertColumn(1);
}

void MainWindow::editMenu()
{
    table->edit(table->selectionModel()->currentIndex());
}

void MainWindow::resetMenu(const QString & str)
{
    if (str.size() == 0)
        tableModel->SetMode(sortMode | orderMode);
    //else
    //    searchMenu();
}

void MainWindow::searchMenu()
{
    if (radioName->isChecked())
        tableModel->Search(editLine->text().toLocal8Bit().data());
    else
        tableModel->Search(editLine->text().toUInt());
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

void MainWindow::resizeEvent(QResizeEvent*)
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

    editLine = new QLineEdit;
    editLine->setStatusTip("Search Record");
    editLine->setClearButtonEnabled(true);
    editLine->setStyleSheet("padding-left:5;");

    connect(editLine ,SIGNAL(returnPressed()), this, SLOT(searchMenu()));
    connect(editLine ,SIGNAL(textChanged(const QString &)), this, SLOT(resetMenu(const QString &)));

    toolbar->addWidget(editLine);

    radioId = new QRadioButton;
    radioId->setText("By ID");
    radioId->setStatusTip("Search By ID");
    radioId->setChecked(true);
    radioId->setStyleSheet("padding-left:5;");
    toolbar->addWidget(radioId);

    radioName = new QRadioButton;
    radioName->setText("By Name");
    radioName->setStatusTip("Search By Name");
    radioName->setStyleSheet("padding-left:5; padding-right:5;");
    toolbar->addWidget(radioName);

    button = new QToolButton;
    button->setIcon(QIcon(":/images/database-search-icon.png"));
    button->setToolTip("Search Record");
    button->setText(button->toolTip());
    button->setStatusTip(button->toolTip());

    connect(button ,SIGNAL(clicked()), this, SLOT(searchMenu()));
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
