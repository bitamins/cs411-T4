#include "mainwindow.h"
#include "sql_db_wrapper.h"
#include <QApplication>
#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
