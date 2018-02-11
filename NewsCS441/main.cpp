#include "mainwindow.h"
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

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("aggregator.ddns.net");
        db.setDatabaseName("aggregator");
        db.setUserName("cs411backendAccess");
        db.setPassword("t4back441");
        bool ok = db.open();
        qDebug() << "DB Opened: " << ok;

        QSqlQuery query(db);
        bool ret = query.exec("SELECT * FROM Business");
        qDebug() << "Data Retrieved: " << ret;

        qDebug() << query.size();
        while (query.next()) {
                QString something = query.value(0).toString();
                qDebug() << something;
            }
    return a.exec();
}
