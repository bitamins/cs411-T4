#ifndef PAGEMANAGER_H
#define PAGEMANAGER_H
#include <QList>
#include <QSqlQuery>
#include <QListWidget>
#include <QLabel>
#include <QWidget>
#include <QGridLayout>
#include <QHash>
#include <QObject>
#include "querybuilder.h"
#include "downloadmanager.h"
class pageManager:public QObject
{
    Q_OBJECT

public:
    void createPages(QSqlQuery, QListWidget*);
    //void loadPage(int, QListWidget*);
    static pageManager* Instance();
    bool isReady();
    ~pageManager();

public slots:
    void setNewsImage(QString);

protected:
    pageManager();

private:
    static pageManager* _instance;
    DownloadManager* CDM;
    QHash<QString,QLabel*> imgHash;
    void downloadNewsImage(QString);
    bool ready;
};

#endif // PAGEMANAGER_H
