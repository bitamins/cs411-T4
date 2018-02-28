#include "querybuilder.h"
#include <QSqlQuery>
#include <QSqlDatabase>

//constructor for query builder, initializes a Qstring to start off with "SELECT"
// and then adds "FROM" and the table name specified by the cat argument

//example usage: QueryBuilder query(columns,"Business");

// now query.query will hold "SELECT * FROM Business"
//the query can either be finalized through finalizeQuery, or it can by limited
//or filter words can be added.

/*
queryBuilder::queryBuilder(QString cat, QSqlDatabase db)
{
   finalQuery = QSqlQuery(db);
   query = "SELECT * FROM " + cat;
}
*/
QueryBuilder::QueryBuilder()
{
;
}

/*
QueryBuilder::QueryBuilder(QSqlDatabase initialDatabase)
{
    database = initialDatabase;
}*/

//allows filter words to be added to a query throught the use of LIKE command
//filterwords are passed as a vector of Qstrings to the function
//the filterwords are then added to the query string
void QueryBuilder::addFilterWords(std::vector<QString> filterWords)
{
    int i = 1;

    query += " WHERE ";
    foreach(QString s, filterWords)
    {

        if(i++ != filterWords.size())//if theres more filter words add an or
            query += "title LIKE '%" + s +"%' OR ";
        else//otherwise just end the string
            query += "title LIKE '%" + s +"%'";

    }
}

// finalizes the string by adding a semi-colon to the end
void QueryBuilder::finalizeQueries()
{
    for(int i =0; i< queries.length();i++)
    {
        queries[i] += ";";
    }
}
//only call this method after finalizeQuery has been called.
//returns the result of the query
QSqlQuery QueryBuilder::execQuery()
{
    finalQueries[0].exec(query);
    return finalQueries[0];
}

std::vector<QSqlQuery> QueryBuilder::execQueries()
{
    foreach(QString tempString, queries)
    {
        QSqlQuery tempQuery(database);
        tempQuery.exec(tempString);
        finalQueries.push_back(tempQuery);
    }
    return finalQueries;
}

void QueryBuilder::clearQueries()
{
    queries.clear();
    finalQueries.clear();
    query = "";
}

void QueryBuilder::addDatabase(QSqlDatabase databaseToAdd)
{
    database = databaseToAdd;
}

void QueryBuilder::initQueries(QStringList listOfCategories)
{

    foreach (QString t, listOfCategories)
    {
        QString temp = "SELECT * FROM " + t;
        queries.append(temp);
    }
}

//allows query to be sorted by date if the argument is true then
//the results will be sorted in ascending order otherwise it will be
//sorted by descending order
void QueryBuilder::sort(bool asc)
{
    for(int i =0; i < queries.length(); i++)
    {
        if(asc)
            queries[i] += " ORDER BY pubDate ASC";
         else
            queries[i] += " ORDER BY pubDate DESC";
    }
}

// allows the amount of results to be limited
void QueryBuilder::limitQuery(QString sizeLim)
{
    query += " LIMIT " + sizeLim;
}
