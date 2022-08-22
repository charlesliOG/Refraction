#include "mainwindow.h"

#include <QApplication>
#include "gamemodel.h"

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);
   GameModel model;
   MainWindow w(model);
   w.show();
   return a.exec();
}

