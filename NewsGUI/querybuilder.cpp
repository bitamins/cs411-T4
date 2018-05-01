#include "querybuilder.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include "querybuilder.h"
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
//constructor for query builder

QueryBuilder::QueryBuilder()
{
    qDebug() << "Initializing queryBuilder";

    containsCategories = false;
    containsFilterWords = false;
    containsSources = false;
    queryString = "";
}



//allows filter words to be added to a query throught the use of LIKE command
//filterwords are passed as a vector of Qstrings to the function
//the filterwords are then added to the query string
void QueryBuilder::addFilterWords(QStringList filterWords)
{
    int i = 1;

    if(!queryString.contains("WHERE"))
        queryString += " WHERE ";
    int insertionIndex = queryString.indexOf("WHERE") + 6;
    if(filterWords.size()>0)
        containsFilterWords = true;
    else
        return;
    queryString.insert(insertionIndex,"(");
    insertionIndex++;
    foreach(QString s, filterWords)
    {

        if(i++ != 1)//if theres more filter words add an or
            queryString.insert(insertionIndex, "title LIKE '%" + s + "%' OR description LIKE '%" + s +"%' OR ");
        else//otherwise just end the string
            queryString.insert(insertionIndex, "title LIKE '%" + s + "%' OR description LIKE '%" + s +"%') AND ");

    }
}

void QueryBuilder::addRowSelection(int start, int distance)
{

    queryString += " LIMIT " + QString::number(start) + "," + QString::number(distance);
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

void QueryBuilder::clearQuery()
{
    queryString = "";
    query.clear();

    final = false;
    containsFilterWords = false;

}

void QueryBuilder::addDatabase(QSqlDatabase databaseToAdd)
{
    database = databaseToAdd;
}

void QueryBuilder::initQuery(QStringList listOfCategories)
{
        int i = 0;
        queryString = "SELECT DISTINCT * FROM News WHERE (";

        foreach(QString category, listOfCategories)
        {
            queryString += "category='" + category +"'";
            if(i++ != listOfCategories.size() - 1)
                 queryString +=" OR ";
        }

        queryString += ")";
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
    queryString += "FROM News";
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

void QueryBuilder::addSources(QStringList sources)
{
    int i = 1;
    if(!queryString.contains("WHERE"))
        queryString += " WHERE ";
    int insertionIndex = queryString.indexOf("WHERE") + 6;
    if(sources.size()<1)
        return;
    queryString.insert(insertionIndex,"(");
    insertionIndex++;
    foreach(QString s, sources)
    {

        if(i++ != 1)//if theres more filter words add an or
            queryString.insert(insertionIndex, "source='" + s + "' OR ");
        else//otherwise just end the string
            queryString.insert(insertionIndex, "source='" + s + "') AND ");

    }
}
void QueryBuilder::filterDate(QDate begin, QDate end)
{
    QString date = "(pubDate BETWEEN '" + begin.toString(Qt::ISODate) + " 00:00:00' AND '" + end.toString(Qt::ISODate) +" 23:59:59') AND ";
    if(!queryString.contains("WHERE"))
        queryString += " WHERE ";
    int insertionIndex = queryString.indexOf("WHERE") + 6;

    queryString.insert(insertionIndex, date);
}

QSqlQuery QueryBuilder::getFinalQuery()
{
    return query;
}
