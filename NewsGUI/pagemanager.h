#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H
#include "newsentry.h"
#include <QList>
#include <QSqlQuery>
#include <QListWidget>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include "querybuilder.h"
#include "newsentry.h"
class pageManager
{
public:
    void createPages(QSqlQuery, QListWidget*);
    //void loadPage(int, QListWidget*);
    static pageManager* Instance();
    int getNumOfPages();

protected:
    pageManager();
private:
    static pageManager* _instance;
    QList<QList<newsEntry>> pages;
};

#endif // PAGEMANAGER_H
