#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QDesktopWidget>
#include "mainwindow.h"

//#include "studenttree.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    AboutDialog about;
    about.show();
    about.setAsSplash(0);
    about.exec();

    MainWindow mainWindow;
    mainWindow.show();

    return a.exec();


    /*StudentTree* tree = new StudentTree;
    for (int i=100; i>=0; i--)
        tree->Insert(i, "Anwar");

    tree->Print(tree->bstId->root);

    int i=51;
    //NODE** node = &(tree->bstId->root);
    while (--i)
    {
        tree->DeleteNode(&(tree->bstId->root));
    //    if (*node)
    //        *node = (*node)->left;
    }

    tree->Print(tree->bstId->root);*/
}
