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
    void addFilterWords(std::vector<QString>);
    void finalizeQuery();
    void limitQuery(QString);
    void addExclusionWords();
    void sort(bool);
    void filterSource();
    void clearQueries();
    void addDatabase(QSqlDatabase databaseToAdd);
    void initQueries(QStringList);
    void initManual(QStringList);
    QSqlQuery getFinalQuery();
    QSqlQuery execQuery();


private:
   bool final;
   QString queryString;
   QSqlQuery query;
   QSqlDatabase database;
};

#endif // QUERYBUILDER_H
