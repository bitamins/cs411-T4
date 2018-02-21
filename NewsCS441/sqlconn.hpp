#ifndef SQLCONN_H
#define SQLCONN_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <string.h>
#include <QString>
using namespace std;
class SQLConn
{
public:

    static SQLConn* Instance();
    bool getStatus();
    QSqlQuery makeQuery(QString, QString);
    void makeConnection(QString, QString);
protected:
    SQLConn();
private:
    QSqlDatabase db;
    static SQLConn* _instance;
    bool status;

};

#endif // SQLCONN_H
