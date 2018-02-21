#include "querybuilder.h"
#include <QSqlQuery>
#include <QSqlDatabase>


queryBuilder::queryBuilder(std::vector<QString> columns, QString cat )
{
    int i = 1;
   query = "SELECT ";
   foreach(QString s, columns)
   {
            if(i++ == columns.size())
                query += (s + " ");
            else
                query += (s + ", ");

   }
   query += "FROM " + cat +" ";
}

void queryBuilder::addFilterWords(std::vector<QString> filterWords)
{
    int i = 1;

    query += "WHERE ";
    foreach(QString s, filterWords)
    {

        if(i++ != filterWords.size())
            query += "title LIKE %" + s +"% OR";
        else
            query += "title LIKE %" + s +"%";

    }
}

void queryBuilder::finalizeQuery()
{
    query += ";";
}

void queryBuilder::limitQuery(QString sizeLim)
{
    query += "LIMIT " + sizeLim;
}
