#include "mainwindow.h"
#include <QApplication>
#include <QSettings>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    if(argc != 3)
    {
           qDebug() << "Username and Password not stored as commandline arguments";
           a.exit();
           return 0;
    }
    //MainWindow w(QString::fromStdString(argv[1]),QString::fromStdString(argv[2]));
    //w.show();
    //return a.exec();
    MainWindow* window = MainWindow::Instance(QString::fromStdString(argv[1]),QString::fromStdString(argv[2]));
    window->show();
    int returned = a.exec();
    delete window;
    return returned;
}
