#include "newsentry.h"

newsEntry::newsEntry()
{
;
}

void newsEntry::setItems(QWidget* newWidget, QListWidgetItem* listItem, QString iUrl)
{
    imageUrl = iUrl;
    newsWidget = newWidget;
    newsItem = listItem;
}

QWidget* newsEntry::getWidget()
{
    return newsWidget;
}

QListWidgetItem* newsEntry:: getItem()
{
    return newsItem;
}

QString newsEntry::getUrl()
{
    return imageUrl;
}
