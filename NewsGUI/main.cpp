#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(QString::fromStdString(argv[1]),QString::fromStdString(argv[2]));
    w.show();

    return a.exec();
}
