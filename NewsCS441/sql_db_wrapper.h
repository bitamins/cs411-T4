#ifndef SQL_DB_WRAPPER_H
#define SQL_DB_WRAPPER_H
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>

using namespace std;

class SQL_DB_WRAPPER
{
private:
    QSqlDatabase db;

public:
    SQL_DB_WRAPPER();
    void get_Database();
    QString get_Query(QString category);
};

#endif // SQL_DB_WRAPPER_H
