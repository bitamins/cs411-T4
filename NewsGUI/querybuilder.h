#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H
#include <vector>
#include <QString>
#include <QSqlQuery>
#include <QStringList>
#include <QDate>
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
    void filterDate(QDate, QDate);
    void clearQuery();
    void addDatabase(QSqlDatabase databaseToAdd);
    void initQuery(QStringList);
    QSqlQuery getFinalQuery();
    QSqlQuery execQuery();
    void addSources(QStringList);
    void addRowSelection(int start, int end);

private:
   QString queryString;
   QSqlQuery query;
   bool final;
   QSqlDatabase database;
   bool containsFilterWords;
   bool containsSources;
   bool containsCategories;
};

#endif // QUERYBUILDER_H
