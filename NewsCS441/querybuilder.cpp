#include "querybuilder.h"
#include <QSqlQuery>
#include <QSqlDatabase>

//constructor for query builder, initializes a Qstring to start off with "SELECT"
// and then adds "FROM" and the table name specified by the cat argument

//example usage: QueryBuilder query(columns,"Business");

// now query.query will hold "SELECT * FROM Business"
//the query can either be finalized through finalizeQuery, or it can by limited
//or filter words can be added.
queryBuilder::queryBuilder(QString cat, QSqlDatabase db)
{
   finalQuery = QSqlQuery(db);
   query = "SELECT * FROM " + cat;
}


//allows filter words to be added to a query throught the use of LIKE command
//filterwords are passed as a vector of Qstrings to the function
//the filterwords are then added to the query string
void queryBuilder::addFilterWords(std::vector<QString> filterWords)
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
void queryBuilder::finalizeQuery()
{
    query += ";";

}
//only call this method after finalizeQuery has been called.
//returns the result of the query
QSqlQuery queryBuilder::execQuery()
{
    finalQuery.exec(query);
    return finalQuery;
}

void queryBuilder::limitQuery(QString sizeLim)
{
    query += " LIMIT " + sizeLim;
}
