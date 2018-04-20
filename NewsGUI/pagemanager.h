#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H
#include <QList>
#include <QSqlQuery>
#include <QListWidget>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include "querybuilder.h"
class pageManager
{
public:
    void createPages(QSqlQuery, QListWidget*);
    static pageManager* Instance();
    void goBackAPage(bool backButtonPressed, QLineEdit* pageNum ,QListWidget* newsListWidget, QueryBuilder queryBuild, int querySize);
protected:
    pageManager();
private:
    static pageManager* _instance;
    int startRow;
    int rowsPerPage;
    int currentPage;
};

#endif // PAGEMANAGER_H
