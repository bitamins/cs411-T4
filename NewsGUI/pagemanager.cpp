#include "pagemanager.h"
#include "customdownloadmanager.h"
#include "querybuilder.h"


pageManager *pageManager::_instance = 0;

pageManager::pageManager()
{
    CDM = CustomDownloadManager::Instance();
    QObject::connect(CDM,SIGNAL(imageDownloaded(QString)),SLOT(setNewsImage(QString)));
}

pageManager::~pageManager(){
    delete _instance;
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
    QString imgFileName = "";
    while(query.next())
    {
        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(0,145));

        //create custom widget item and layout
        QWidget *newWidget = new QWidget();
        QLayout *newGrid = new QGridLayout();
        auto *newHLayout = new QHBoxLayout();

        //create item labels
        QLabel *titleLabel = new QLabel("Title: " + query.value(TITLE).toString());
        QLabel *destLabel = new QLabel("Description: " + query.value(DESCRIPTION).toString());
            destLabel->setWordWrap(true);
        QLabel *srcLabel = new QLabel("Source: " + query.value(SOURCE).toString());
        QLabel *datLabel = new QLabel("Date: " + query.value(DATE).toDateTime().toString(dateFormat));
        QLabel *catLabel = new QLabel("Category: " + query.value(CATEGORY).toString());
        QLabel *picLabel = new QLabel();
        picLabel->setMaximumSize(100,100);
        picLabel->setMinimumSize(100,100);

        imgFileName = CDM->saveFileName(QUrl(query.value(IMAGE).toString()));
        downloadNewsImage(query.value(IMAGE).toString());
        imgHash[imgFileName] = picLabel;


        newGrid->setSpacing(0);
        newGrid->addWidget(titleLabel);
        newGrid->addWidget(destLabel);
        newGrid->addWidget(srcLabel);
        newGrid->addWidget(datLabel);
        newGrid->addWidget(catLabel);

        newHLayout->addWidget(picLabel);
        newHLayout->addLayout(newGrid);
        newWidget->setLayout(newHLayout);

        item->setData(3, query.value(URL).toString());
        newsList->addItem(item);
        newsList->setItemWidget(item, newWidget);
    }
}

void pageManager::downloadNewsImage(QString url){
    CDM->startDownload(QUrl(url));
    //qDebug() << "downloading: " << url << endl;
}

void pageManager::setNewsImage(QString imageName){
    //qDebug() << "setting: " << imageName << endl;
    try{
        QLabel* picLabel = imgHash[imageName];

        QPixmap img(imageName);
        QSize imgSize(100,100);
        QPixmap smallerImg = img.scaled(imgSize,Qt::KeepAspectRatio);

        picLabel->setPixmap(smallerImg);
    }
    catch(...){
        qDebug() << "image not set: " << imageName;
    }

}

