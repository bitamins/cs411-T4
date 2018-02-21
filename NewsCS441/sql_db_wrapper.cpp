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
<<<<<<< HEAD
    QString command = "SELECT * FROM ";
           command = command + category;
    qDebug() << "Data Retrieved: " << query.exec(command);
=======
    string command = "SELECT * FROM ";
           command = command.append(category);
    QString qcommand = QString::fromStdString(command);
    qDebug() << "Data Retrieved: " << query.exec(qcommand);
>>>>>>> qtmichael
    qDebug() << query.size();
    QString qret = query.value(0).toString();

<<<<<<< HEAD

    return query.value(1).toString();
}

=======
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
>>>>>>> qtmichael
