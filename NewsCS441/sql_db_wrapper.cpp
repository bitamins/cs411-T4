#include "sql_db_wrapper.h"
#include <QDebug>

SQL_DB_WRAPPER::SQL_DB_WRAPPER()
{

}

void SQL_DB_WRAPPER::get_Database(){ //Opens database
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("aggregator.ddns.net");
    db.setDatabaseName("aggregator");
    db.setUserName(""); //Hid name for now
    db.setPassword(""); //Hid pass for now
    qDebug() << "DB Opened: " << db.open();
}

QString SQL_DB_WRAPPER::get_Query(QString category){
    QSqlQuery query(db);
    QString command = "SELECT * FROM ";
           command = command + category;
    qDebug() << "Data Retrieved: " << query.exec(command);
    qDebug() << query.size();


    return query.value(1).toString();
}

