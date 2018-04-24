#include "mainwindow.h"
#include "ui_mainwindow.h"

QStringList parseList(QString);
QSqlDatabase startDb(QString, QString);
void errorCheck(QSqlDatabase);

MainWindow* MainWindow::_instance = 0;

MainWindow* MainWindow::Instance(QString username, QString pass){
    if ( _instance == 0 )
       _instance = new MainWindow(username,pass);
    return _instance;
}

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

    startRow = 0;
    rowsPerPage = 5;
    querySize = 0;
    currentPage = 0;
    ui->pageNum->setText("Page " + QString::number(currentPage + 1));
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

    driver();

    settingsGrid->addWidget(ui->settingsGroupBox);
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
    delete _instance;
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
    activeCategories.clear();
    activeSources.clear();
    filterList.clear();
    begin = resetDate;
    end = resetDate;
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

void MainWindow::constructQueryWithLimit() {
    queryBuilder.clearQuery();
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
    queryBuilder.addRowSelection(startRow, rowsPerPage);
    queryBuilder.finalizeQuery();
}

int MainWindow::getTotalQuerySizeBeforeLimit()
{
    queryBuilder.clearQuery();
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
    QSqlQuery the_query = queryBuilder.execQuery();
    return the_query.size();
}

void MainWindow::on_updateSettingsButton_clicked()
{
    currentPage = 0;
    startRow = 1;
    ui->pageNum->setText("Page " + QString::number(currentPage + 1));
    begin = ui->fromDateEdit->date();
    end = ui->toDateEdit->date();
    qDebug() << "begin: " <<  begin.toString() << " end: " << end.toString();
    if(begin > end)
    {
        QMessageBox::warning(this,tr("NewsGui"),tr("Begin date must be prior to end date! Please change and hit update again."), QMessageBox::Ok);
        return;
    }
    qDebug() << "Limiting dates: " << limitDate;

    ui->newsListWidget->clear();
    querySize = getTotalQuerySizeBeforeLimit();
    qDebug() << "Query Size: " << querySize;
    constructQueryWithLimit();
    QSqlQuery query = queryBuilder.execQuery();
    try{
        //pageManager::Instance();
        pageManager::Instance()->createPages(query,ui->newsListWidget);
    }
    catch(...)
    {
        qDebug() <<"fire the intern";
    }

    settingsWindow.close();

}


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

void MainWindow::on_dateCheckBox_stateChanged()
{
    if(ui->dateCheckBox->checkState() == Qt::Checked)
        limitDate = true;
    else
        limitDate = false;

}

void MainWindow::goBackAPage(bool backButtonPressed)
{
    ui->newsListWidget->clear();
    int leftOffset = startRow - rowsPerPage;
    if(backButtonPressed){ // Back a page also for negative numbers getting modded
        startRow = (leftOffset % querySize + querySize) % querySize;
        currentPage = ((currentPage - 1) % querySize + querySize) % querySize;
        ui->pageNum->setText("Page " + QString::number(currentPage + 1));
    }
    else { // Going forward a page
        startRow = (startRow + rowsPerPage) % querySize;
        currentPage = (currentPage + 1) % querySize;
        ui->pageNum->setText("Page "+ QString::number(currentPage + 1));
    }
    constructQueryWithLimit();
    QSqlQuery query = queryBuilder.execQuery();
    try{
        pageManager::Instance();
        pageManager::Instance()->createPages(query,ui->newsListWidget);
    }
    catch(...)
    {
        qDebug() <<"fire the intern";
    }
}


void MainWindow::on_NextPage_clicked()
{
    goBackAPage(false);
}

void MainWindow::on_GoBack_clicked()
{
    goBackAPage(true);
}

int MainWindow::extractPageNum() {
    QString pageLabel = ui->pageNum->text();

    pageLabel.replace(" ", "");
    pageLabel.replace("Page", "");
    return pageLabel.toInt();
}

void MainWindow::goToPageEntered()
{
    int pageNumber = extractPageNum();
    if(pageNumber < 1) {
        QMessageBox msgBox;
        msgBox.setText("Please enter a valid number. Page Format: Page <PageNumber>");
        msgBox.exec();
    }
    else if((pageNumber + rowsPerPage) > querySize){
        QMessageBox msgBox;
        msgBox.setText("Page out of range.");
        msgBox.exec();
    }
    else {
        startRow = pageNumber - 1; //Since row 0 exists
        currentPage = pageNumber - 1;
        ui->newsListWidget->clear();
        constructQueryWithLimit();
        QSqlQuery query = queryBuilder.execQuery();
        pageManager::Instance();
        pageManager::Instance()->createPages(query,ui->newsListWidget);
    }
}


void MainWindow::on_pageNum_returnPressed()
{
    goToPageEntered();
}
