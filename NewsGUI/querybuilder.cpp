#include "querybuilder.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
//constructor for query builder, initializes a Qstring to start off with "SELECT"
// and then adds "FROM" and the table name specified by the cat argument

//example usage: QueryBuilder query(columns,"Business");

// now query.query will hold "SELECT * FROM Business"
//the query can either be finalized through finalizeQuery, or it can by limited
//or filter words can be added.

QueryBuilder::QueryBuilder()
{
    qDebug() << "Initializing queryBuilder";
}



//allows filter words to be added to a query throught the use of LIKE command
//filterwords are passed as a vector of Qstrings to the function
//the filterwords are then added to the query string
void QueryBuilder::addFilterWords(QStringList filterWords)
{
    int i = 1;

    queryString += " WHERE ";
    foreach(QString s, filterWords)
    {

        if(i++ != filterWords.size())//if theres more filter words add an or
            queryString += "title LIKE '%" + s +"%' OR ";
        else//otherwise just end the string
            queryString += "title LIKE '%" + s +"%'";

    }
}

// finalizes the string by adding a semi-colon to the end
void QueryBuilder::finalizeQuery()
{
        queryString += ";";
        final = true;
}

//only call this method after finalizeQuery has been called.
//returns the result of the query
QSqlQuery QueryBuilder::execQuery()
{
    if(final)
    {
        query = QSqlQuery(database);
        query.exec(queryString);
        return query;
    }
    return query;
}

void QueryBuilder::clearQueries()
{
    queryString = "";
    query.clear();
}

void QueryBuilder::addDatabase(QSqlDatabase databaseToAdd)
{
    database = databaseToAdd;
}

void QueryBuilder::initQueries(QStringList listOfCategories)
{
        int i = 0;
        queryString = "SELECT DISTINCT * FROM News WHERE ";
        foreach(QString category, listOfCategories)
        {
            queryString += "category='" + category +"'";
            if(i++ != listOfCategories.size() - 1)
                 queryString +=" OR ";
        }


}

void QueryBuilder::initManual(QStringList columns)
{
    int i = 0;
    queryString = "SELECT DISTINCT ";
    foreach(QString column, columns)
    {
        if(i++ != columns.size() -1)
            queryString += column + ", ";
        else
            queryString += column + " ";
    }
}

//allows query to be sorted by date if the argument is true then
//the results will be sorted in ascending order otherwise it will be
//sorted by descending order
void QueryBuilder::sort(bool ascending)
{
        if(ascending)
            queryString += " ORDER BY pubDate ASC";
         else
            queryString += " ORDER BY pubDate DESC";
}

// allows the amount of results to be limited
void QueryBuilder::limitQuery(QString sizeLim)
{
    queryString += " LIMIT " + sizeLim;
}

QSqlQuery QueryBuilder::getFinalQuery()
{
    return query;
}
