#include "sqlconn.hpp"
#include <QString>
using namespace  std;
SQLConn* SQLConn::_instance = 0;

SQLConn* SQLConn::Instance()
{
 if ( _instance == 0 )
    _instance = new SQLConn();
 return _instance;
}

SQLConn::SQLConn()
{
   db = QSqlDatabase::addDatabase("QMYSQL");
}

void SQLConn::makeConnection(QString userName, QString pass)
{
    db.setHostName("aggregator.ddns.net");
    db.setDatabaseName("aggregator");
    db.setUserName(userName);
    db.setPassword(pass);
    status = db.open();
}

bool SQLConn::getStatus()
{
    return status;
}

QSqlQuery SQLConn::makeQuery(QString category, QString limit )
{

    QString query = "SELECT title, author, source, image, description FROM ";
    query += category;
    if(limit == "0")
        query += ";";
    else
        query = query + " LIMIT " + limit + ";";
    QSqlQuery q(db);
    q.exec(query);
    return q;
}
