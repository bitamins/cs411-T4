#include "mainwindow.h"
#include "sql_db_wrapper.h"
#include <QApplication>
#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

QSqlDatabase get_Database(){
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("aggregator.ddns.net");
        db.setDatabaseName("aggregator");
        db.setUserName(""); //Hid name for now
        db.setPassword(""); //Hid pass for now
        bool ok = db.open();
        qDebug() << "DB Opened: " << ok;
        return db;

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("aggregator.ddns.net");
        db.setDatabaseName("aggregator");
        db.setUserName(""); //Hid name for now
        db.setPassword(""); //Hid pass for now
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
