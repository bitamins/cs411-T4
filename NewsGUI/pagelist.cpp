#include "pagelist.hpp"

pageList::pageList()
{
   //qDebug() << "Creating pageList object";
}

QListWidget* pageList::loadPage(int pageNumber)
{
    if(pageNumber >= 0 && pageNumber < pageCount)
        return listOfPages.at(pageNumber);
    else
        return NULL;

}

void pageList::setPages(QSqlQuery query, int pageSize)
{
    //set the page count to the rounded up number of the amount of results divided by page size
    pageCount = ceil(query.size()/pageSize);
    int j = 0;
    for(int i = 0; i < pageCount; i++)
    {
        QListWidget *pageToBeAdded = new QListWidget();
        while(j++ < pageSize && query.next())
        {
            QListWidgetItem *item = new QListWidgetItem();

            item->setSizeHint(QSize(0,100));

            QWidget *newWidget = new QWidget();
            QLayout *newGrid = new QGridLayout();

            QLabel *titleLabel = new QLabel("Title: " + query.value(TITLE).toString());
            QLabel *destLabel = new QLabel("Description: " + query.value(DESCRIPTION).toString());
            QLabel *srcLabel = new QLabel("Source: " + query.value(SOURCE).toString());
            //QLabel *picLabel = new QLabel("Picture: " + query.value(IMAGE).toString());
            QLabel *datLabel = new QLabel("Date: " + query.value(DATE).toString());
            QLabel *catLabel = new QLabel("Category: " + query.value(CATEGORY).toString());

            newGrid->addWidget(titleLabel);
            newGrid->addWidget(destLabel);
            newGrid->addWidget(srcLabel);
            //newGrid->addWidget(picLabel);
            newGrid->addWidget(datLabel);
            newGrid->addWidget(catLabel);
            newWidget->setLayout(newGrid);
            pageToBeAdded->addItem(item);
            pageToBeAdded->setItemWidget(item,newWidget);
            item->setData(3, query.value(URL).toString());
        }
        j = 0;
        listOfPages.append(pageToBeAdded);
    }
}


