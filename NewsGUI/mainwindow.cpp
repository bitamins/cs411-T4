#include "mainwindow.h"
#include "ui_mainwindow.h"

QStringList parseList(QString);
QSqlDatabase startDb(QString, QString);
void errorCheck(QSqlDatabase);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::MainWindow(QString username, QString pass, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("RealNews", "NewsFetcher")
{
    ui->setupUi(this);
    limitDate = false;
    //create Settings separate window
    //QWidget* settingsWindow = new QWidget();
    QLayout* settingsGrid = new QGridLayout();
    settingsWindow.setWindowFlags(Qt::Window);
    settingsWindow.setLayout(settingsGrid);

    database = startDb(username,pass);
    //Retrieve categories settings and settings
    activeSources = settings.value("SourcesList").value<QStringList>();
    activeCategories = settings.value("CategoriesList").value<QStringList>();
    filterList = settings.value("FilterList").value<QStringList>();
    limitDate = settings.value("LimitDate").value<bool>();
    if(limitDate)
    {
        ui->dateCheckBox->setChecked(true);
        begin = settings.value("BeginDate").value<QDate>();
        end = settings.value("EndDate").value<QDate>();
        ui->fromDateEdit->setDate(begin);
        ui->toDateEdit->setDate(end);
    }
    //ui->categoriesLineEdit->setText(settings.value("CategoriesFilterText", "").toString());
    //ui->sourcesLineEdit->setText(settings.value("SourcesFilterText", "").toString());

    driver();

    settingsGrid->addWidget(ui->settingsGroupBox);
    bool test=false;
}

MainWindow::~MainWindow()
{
    //settings.setValue("CategoriesFilterText", ui->categoriesLineEdit->text());
    //settings.setValue("SourcesFilterText", ui->sourcesLineEdit->text());
    settings.setValue("SourcesList", QVariant::fromValue(activeSources));
    settings.setValue("CategoriesList", QVariant::fromValue(activeCategories));
    settings.setValue("FilterList", QVariant::fromValue(filterList));
    settings.setValue("LimitDate", QVariant::fromValue(limitDate));
    if(limitDate)
    {
        settings.setValue("BeginDate", QVariant::fromValue(begin));
        settings.setValue("EndDate", QVariant::fromValue(end));
    }
    delete ui;
}
void MainWindow::driver()
{
    errorCheck(database);
    queryBuilder.addDatabase(database);
    setupCategories();
    setupSources();
    restoreSettings();
}
void MainWindow::setupCategories()
{
    QStringList categories = {"Sports", "Entertainment", "Technology", "Business", "Health", "Science"};
    QStringListIterator categoriesIterator(categories);
    while(categoriesIterator.hasNext())
    {
        QListWidgetItem *listItem = new QListWidgetItem(categoriesIterator.next());
        listItem->setCheckState(Qt::Unchecked);
        ui->categoryListWidget->addItem(listItem);
    }
}

void MainWindow::setupSources()
{
    QStringList columns = {"source"};
    queryBuilder.initManual(columns);
    queryBuilder.finalizeQuery();
    QSqlQuery sourcesQuery = queryBuilder.execQuery();

    while(sourcesQuery.next())
        sources.append(sourcesQuery.value(0).toString());
    sources.sort();

    QStringListIterator sourceIterator(sources);

    while(sourceIterator.hasNext())
    {
        QListWidgetItem *listItem = new QListWidgetItem(sourceIterator.next());
        listItem->setCheckState(Qt::Unchecked);
        ui->sourcesListWidget->addItem(listItem);
    }

}

void MainWindow::on_clearSettingsButton_clicked()
{
    //clear sources list
    //clear categories list
    QDate resetDate = QDate::fromString("01/01/2000", "dd/MM/yyyy");
    ui->sourcesLineEdit->clear();
    ui->newsListWidget->clear();
    queryBuilder.clearQuery();
    ui->dateCheckBox->setCheckState(Qt::Unchecked);
    ui->toDateEdit->setDate(resetDate);
    ui->fromDateEdit->setDate(resetDate);
    foreach(QString category , activeCategories)
    {
        QListWidgetItem* widget = ui->categoryListWidget->findItems(category, Qt::MatchExactly)[0];
        widget->setCheckState(Qt::Unchecked);
    }
    foreach(QString category, activeSources)
    {
        QListWidgetItem* widgetSource = ui->sourcesListWidget->findItems(category,Qt::MatchExactly)[0];
        widgetSource->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::restoreSettings()
{
    if(ui->sourcesListWidget->count() > 0)
    {
        foreach(QString source, activeSources)
        {
            QListWidgetItem* widget = ui->sourcesListWidget->findItems(source, Qt::MatchExactly)[0];
            widget->setCheckState(Qt::Checked);
        }
    }

    if(ui->categoryListWidget->count() > 0)
    {
        foreach(QString category, activeCategories)
        {
            QListWidgetItem* widget = ui->categoryListWidget->findItems(category, Qt::MatchExactly)[0];
            widget->setCheckState(Qt::Checked);
        }
    }
    on_updateSettingsButton_clicked();
}

void MainWindow::downloadNewsImage(QString url){
    CDM.startDownload(QUrl(url));
    qDebug() << "downloading " << url << endl;
}

void MainWindow::setNewsImages(){

}

void MainWindow::on_updateSettingsButton_clicked()
{
    begin = ui->fromDateEdit->date();
    end = ui->toDateEdit->date();
    qDebug() << "begin: " <<  begin.toString() << " end: " << end.toString();
    if(begin > end)
    {
        QMessageBox::warning(this,tr("NewsGui"),tr("Begin date must be prior to end date! Please change and hit update again."), QMessageBox::Ok);
        return;
    }
    qDebug() << "Limiting dates: " << limitDate;
    queryBuilder.clearQuery();
    ui->newsListWidget->clear();
    //get sources list
    QStringList sourceList = activeSources;
    //get categories list
    QStringList categoryList = activeCategories;
    QStringList filter = parseList(ui->filterLineEdit->text());
    //query the data base for the list of news
    queryBuilder.initQuery(categoryList);
    //update the news list with the new news
    queryBuilder.sort(false);
    queryBuilder.addFilterWords(filter);
    queryBuilder.addSources(sourceList);
    if(limitDate)
        queryBuilder.filterDate(begin,end);
    queryBuilder.finalizeQuery();
    QSqlQuery query = queryBuilder.execQuery();
    QString dateFormat = "dddd, MMMM d, yyyy"; //day of week, month, day num, year
    while(query.next())
    {

        QListWidgetItem *item = new QListWidgetItem();
        item->setSizeHint(QSize(0,145));

        //create custom widget item and layout
        QWidget *newWidget = new QWidget();
        QLayout *newGrid = new QGridLayout();

        //create item labels
        QLabel *titleLabel = new QLabel("Title: " + query.value(TITLE).toString());
        QLabel *destLabel = new QLabel("Description: " + query.value(DESCRIPTION).toString());
        destLabel->setWordWrap(true);
        QLabel *srcLabel = new QLabel("Source: " + query.value(SOURCE).toString());
        //QLabel *picLabel = new QLabel("Picture: " + query.value(IMAGE).toString());
        QLabel *datLabel = new QLabel("Date: " + query.value(DATE).toDateTime().toString(dateFormat));
        QLabel *picLabel = new QLabel();

       /*
        if(!test){
            picLabel->setPixmap(CDM.downloadImage(QUrl(query.value(IMAGE).toString())));
            test = true;
        }
         */
        downloadNewsImage(query.value(IMAGE).toString());//start the news image downloads

        QLabel *catLabel = new QLabel("Category: " + query.value(CATEGORY).toString());
       // QLabel *blankLabel = new QLabel("thing");

        newGrid->setSpacing(0);
        newGrid->addWidget(titleLabel);
        newGrid->addWidget(destLabel);
        newGrid->addWidget(srcLabel);
        newGrid->addWidget(picLabel);
        newGrid->addWidget(datLabel);
        newGrid->addWidget(catLabel);
        //newGrid->addWidget(blankLabel,0,0)
        newWidget->setLayout(newGrid);
        ui->newsListWidget->addItem(item);
        ui->newsListWidget->setItemWidget(item,newWidget);
        item->setData(3, query.value(URL).toString());

    }
    settingsWindow.close();

}

/*
//retrieves an image from a url for newslist items
void getImageFromUrl(QString newsUrl)
{
   //create network interface object
   //connect a slot for finished download flags
   QNetworkAccessManager *nam = new QNetworkAccessManager();
   QObject::connect(nam, &QNetworkAccessManager::finished, &MainWindow::downloadFinished);

   //create network request then send it
   const QUrl url = QUrl(newsUrl);
   nam->get(request);
}

QPixmap downloadFinished(QNetworkReply *reply)
{
    QPixmap pm;
    pm.loadFromData(reply.readAll());
    delete reply;
    return pm;
}
*/
//takes a Qstring with the given format "word1,word2,word3"
//then splits it based on commas and returns the result as a QstringList which is
//essentially a string array
QStringList parseList(QString list)
{
    return list.split(",");
}

//makes a connection to the database with the specified username and password passed
//as parameters to this function, the QSqlDatabase object is then returned
QSqlDatabase startDb(QString user, QString pass)
{
    //creates SQLConn object pointer
    SQLConn::Instance();
    //makes connection to db with the username and pass specified by parameters
    SQLConn::Instance()->makeConnection(user,pass);
    //returns the QSqlDatabase object that was created by the SqlConn object
    return SQLConn::Instance()->getDatabase();
}

//checks to make sure the database passed as a parameter is open
//displays positive message if open, negative message with
//error is displayed otherwise
void errorCheck(QSqlDatabase db)
{
    if(db.isOpen())
        qDebug() << "Created db connection";
    else
    {
        qDebug() << "Failed to connect to database";
        qDebug() << "Databse error: " << db.lastError();
    }
}

//opens the news article that was clicked in the default browser.
void MainWindow::on_newsListWidget_itemClicked(QListWidgetItem *item)
{
    QString link = item->data(3).toString();
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_actionSettings_triggered()
{
    settingsWindow.exec();
}

void MainWindow::on_categoryListWidget_itemChanged(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked && !activeCategories.contains(item->text()))
            activeCategories.append(item->text());
        else if(item->checkState() == Qt::Unchecked)
            activeCategories.removeAt(activeCategories.indexOf(item->text()));
}

void MainWindow::on_sourcesListWidget_itemChanged(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked && !activeSources.contains(item->text()))
            activeSources.append(item->text());
        else if(item->checkState() == Qt::Unchecked)
            activeSources.removeAt(activeSources.indexOf(item->text()));
}

void MainWindow::on_filterLineEdit_returnPressed()
{
    QString filterText = ui->filterLineEdit->text();
    filterList = parseList(filterText);
    on_updateSettingsButton_clicked();
}

void MainWindow::on_sourcesLineEdit_textEdited(const QString &arg1)
{
    QRegExp searchTerm(".*" + arg1 + ".*");
    QList<QListWidgetItem *> widgets = ui->sourcesListWidget->findItems(searchTerm.pattern(), Qt::MatchRegExp);
    if(widgets.size() > 0)
    {
        ui->sourcesListWidget->setCurrentItem(widgets[0]);
        ui->sourcesListWidget->scrollToItem(widgets[0],QAbstractItemView::PositionAtTop);
    }
    else
    {
        qDebug() << "couldn't find " << arg1;
        ui->sourcesListWidget->setCurrentRow(0);
    }
}

void MainWindow::on_dateCheckBox_stateChanged(int arg1)
{
    if(ui->dateCheckBox->checkState() == Qt::Checked)
        limitDate = true;
    else
        limitDate = false;

}

