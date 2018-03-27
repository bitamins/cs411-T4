#ifndef PAGELIST_H
#define PAGELIST_H
#include "page.h"
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include "querybuilder.h"
class pageList
{
    public:
        pageList();
        QListWidget* loadPage(int);
        void setPages(QSqlQuery, int);
    private:
        QList<QListWidget*> listOfPages;

};

#endif // PAGELIST_H
