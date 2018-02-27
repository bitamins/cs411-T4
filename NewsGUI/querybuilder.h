#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H
#include <vector>
#include <QString>
#include <QSqlQuery>
class queryBuilder
{
public:
    queryBuilder(QString, QSqlDatabase);
    void addFilterWords(std::vector<QString>);
    void finalizeQuery();
    void limitQuery(QString sizeLim);
    QSqlQuery execQuery();
    void addExclusionWords();
    void sort(bool);
private:
   QString query;
   QSqlQuery finalQuery;
};

#endif // QUERYBUILDER_H
