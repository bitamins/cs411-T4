#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H
#include <vector>
#include <QString>
#include <QSqlQuery>
#include <QStringList>
enum COLUMNS{ID, TITLE, SOURCE, AUTHOR, URL, IMAGE, DATE, DESCRIPTION, CATEGORY};
class QueryBuilder
{
public:

    QueryBuilder();
    void addFilterWords(QStringList);
    void finalizeQuery();
    void limitQuery(QString);
    std::vector<QSqlQuery> execQueries();
    void addExclusionWords();
    void sort(bool);
    void initManual(QStringList);
    void filterSource();
    void clearQueries();
    void addDatabase(QSqlDatabase databaseToAdd);
    void initQueries(QStringList);
    QSqlQuery getFinalQuery();
    QSqlQuery execQuery();


private:
   QString queryString;
   QSqlQuery query;
   bool final;
   QSqlDatabase database;
};

#endif // QUERYBUILDER_H
