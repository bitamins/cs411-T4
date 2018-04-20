#include "pagemanager.h"
pageManager *pageManager::_instance = 0;
pageManager::pageManager()
{
      startRow = 0;
      rowsPerPage = 5;
      currentPage = 0;
}

pageManager* pageManager::Instance()
{
    if(_instance == NULL)
        _instance = new pageManager();
    return _instance;
}

void pageManager::createPages(QSqlQuery query, QListWidget* newsList)
{
    QString dateFormat = "dddd, MMMM d, yyyy"; //day of week, month, day num, year
    while(query.next())
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(0,145));

        //create custom widget item and layout
        QWidget *newWidget = new QWidget();
        QLayout *newGrid = new QGridLayout();

        //create item labels
        QLabel *titleLabel = new QLabel("Title: <strong>" + query.value(TITLE).toString() + "</strong>");
        QLabel *destLabel = new QLabel("Description: " + query.value(DESCRIPTION).toString());
        destLabel->setWordWrap(true);
        QLabel *srcLabel = new QLabel("Source: " + query.value(SOURCE).toString());
        QLabel *picLabel = new QLabel("Picture: " + query.value(IMAGE).toString());
        QLabel *datLabel = new QLabel("Date: " + query.value(DATE).toDateTime().toString(dateFormat));

        QLabel *catLabel = new QLabel("Category: " + query.value(CATEGORY).toString());
       // QLabel *blankLabel = new QLabel("thing");

        newGrid->setSpacing(0);
        newGrid->addWidget(titleLabel);
        newGrid->addWidget(destLabel);
        newGrid->addWidget(srcLabel);
        newGrid->addWidget(picLabel);
        newGrid->addWidget(datLabel);
        newGrid->addWidget(catLabel);
        newGrid->addWidget(picLabel);
        //newGrid->addWidget(blankLabel,0,0)
        newWidget->setLayout(newGrid);
        item->setData(3, query.value(URL).toString());

        newsList->addItem(item);
        newsList->setItemWidget(item, newWidget);
    }
}

void pageManager::goBackAPage(bool backButtonPressed, QLineEdit* pageNum ,QListWidget* newsListWidget, QueryBuilder queryBuild, int querySize)
{
    newsListWidget->clear();
    int leftOffset = startRow - rowsPerPage;
    if(backButtonPressed){ // Back a page also for negative numbers getting modded
        startRow = (leftOffset % querySize + querySize) % querySize;
        currentPage = ((currentPage - 1) % querySize + querySize) % querySize;
        pageNum->setText("Page " + QString::number(currentPage + 1));
    }
    else { // Going forward a page
        startRow = (startRow + rowsPerPage) % querySize;
        currentPage = (currentPage + 1) % querySize;
        pageNum->setText("Page "+ QString::number(currentPage + 1));
    }
    constructQueryWithLimit();
    QSqlQuery query = queryBuild.execQuery();

    createPages(query,ui->newsListWidget);

}
