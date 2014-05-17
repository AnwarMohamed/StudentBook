#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QMessageBox>
#include <QCloseEvent>
#include <QTableWidget>
#include <QActionGroup>
#include <QSignalMapper>
#include <QToolBar>
#include <QToolButton>
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
#define VIEW_ORDER_PRE  1
#define VIEW_ORDER_IN   2
#define VIEW_ORDER_POST 3
#define SORT_BY_ID      1
#define SORT_BY_NAME    2

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void aboutDialog();
    void changeViewOrder(int orderType);
    void changeViewSort(int sortType);

private:
    Ui::MainWindow *ui;
    void setupStatusBar();
    void setupToolbar();

    bool dataSaved;
    QTableView* table;
    QToolBar* toolbar;
    QActionGroup* orderGroup, *sortGroup;
    QSignalMapper* orderSignalMapper, *sortSignalMapper;

    void initVariables();
    bool checkSaveMessage();


protected:
    void resizeEvent(QResizeEvent * event);
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
