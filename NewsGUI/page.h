#ifndef PAGE_H
#define PAGE_H
#include <QListWidget>
#include <QList>
#include <QSqlQuery>
#include <math.h>
class page
{
    public:
        page(QListWidget*);
        QListWidget* getPage();
    private:
        QListWidget* pageList;

};

#endif // PAGE_H
