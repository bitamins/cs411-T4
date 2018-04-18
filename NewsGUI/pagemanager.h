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
    //void loadPage(int, QListWidget*);
    static pageManager* Instance();

protected:
    pageManager();
private:
    static pageManager* _instance;
};

#endif // PAGEMANAGER_H
