#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H
#include <vector>
#include <QString>
class queryBuilder
{
public:
    queryBuilder(std::vector<QString>, QString);
    void addFilterWords(std::vector<QString>);
    void finalizeQuery();
    void limitQuery(QString sizeLim);


private:
   QString query;
};

#endif // QUERYBUILDER_H
