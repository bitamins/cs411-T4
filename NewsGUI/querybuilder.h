#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H
#include <vector>
#include <QString>
#include <QSqlQuery>
#include <QStringList>
class QueryBuilder
{
public:

    QueryBuilder();
    void addFilterWords(std::vector<QString>);
    void finalizeQueries();
    void limitQuery(QString);
    std::vector<QSqlQuery> execQueries();
    void addExclusionWords();
    void sort(bool);
    void filterSource();
    void clearQueries();
    void addDatabase(QSqlDatabase databaseToAdd);
    void initQueries(QStringList);
    QSqlQuery execQuery();


private:
   QString query;
   QStringList queries;
   std::vector<QSqlQuery> finalQueries;
   QSqlDatabase database;
};

#endif // QUERYBUILDER_H
