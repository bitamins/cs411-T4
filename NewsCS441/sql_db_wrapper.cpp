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
    QString command = "SELECT * FROM ";
           command = command.append(category);
    qDebug() << "Data Retrieved: " << query.exec(command);
    qDebug() << query.size();


    return query.value(0).toString();
//    vector<string> tempVec;
//    while(query.next()){
//        tempVec.append(query.value(0).toString())
//    }
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
