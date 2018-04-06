#ifndef NEWSENTRY_H
#define NEWSENTRY_H
#include <QWidget>
#include <QListWidgetItem>

class newsEntry
{
    public:
        newsEntry();
        void setItems(QWidget*, QListWidgetItem*, QString);
        QWidget *getWidget();
        QListWidgetItem * getItem();
        QString getUrl();

    private:
        QWidget *newsWidget;
        QListWidgetItem *newsItem;
        QString imageUrl;
};

#endif // NEWSENTRY_H
