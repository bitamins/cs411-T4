#include "sql_db_wrapper.h"

SQL_DB_WRAPPER::SQL_DB_WRAPPER()
{

}

void SQL_DB_WRAPPER::get_Database(){
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("aggregator.ddns.net");
    db.setDatabaseName("aggregator");
    db.setUserName(""); //Hid name for now
    db.setPassword(""); //Hid pass for now
    qDebug() << "DB Opened: " << db.open();
}

string SQL_DB_WRAPPER::get_Query(string category){
    QSqlQuery query(db);
    string command = "SELECT * FROM ";
           command = command.append(category);
    QString qcommand = QString::fromStdString(command);
    qDebug() << "Data Retrieved: " << query.exec(qcommand);
    qDebug() << query.size();
    QString qret = query.value(0).toString();

    return qret.toStdString();
}
/*

QSqlQuery query(db);
bool ret = query.exec("SELECT * FROM Business");
qDebug() << "Data Retrieved: " << ret;

qDebug() << query.size();
while (query.next()) {
        QString something = query.value(0).toString();
        qDebug() << something;
    }
 */
