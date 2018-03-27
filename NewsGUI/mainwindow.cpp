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
    QLayout* settingsGrid = new QGridLayout();
    settingsWindow.setWindowFlags(Qt::Window);
    settingsWindow.setLayout(settingsGrid);

    database = startDb(username,pass);

    driver();

    settingsGrid->addWidget(ui->settingsGroupBox);
}
//saves settings upon the closing of the window. Only saves the date range if
//the date checkbox was checked upon closing
MainWindow::~MainWindow()
{

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

//calls the majority of the helper functions, used simply to reduce the body of the constructor
void MainWindow::driver()
{
    errorCheck(database);
    queryBuilder.addDatabase(database);
    setupCategories();
    setupSources();
    restoreSettings();
}

//adds category checkboxes to settings ui, puts them in unchecked state
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
    ui->sourcesLineEdit->clear();
    ui->newsListWidget->clear();
    queryBuilder.clearQuery();

}

void MainWindow::restoreSettings()
{
    activeSources = settings.value("SourcesList").value<QStringList>();
    activeCategories = settings.value("CategoriesList").value<QStringList>();
    filterList = settings.value("FilterList").value<QStringList>();

    limitDate = settings.value("LimitDate").value<bool>();

    //only load date from settings if limitDate was checked on last program run
    if(limitDate)
    {
        ui->dateCheckBox->setChecked(true);
        begin = settings.value("BeginDate").value<QDate>();
        end = settings.value("EndDate").value<QDate>();
        ui->fromDateEdit->setDate(begin);
        ui->toDateEdit->setDate(end);
    }
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
void MainWindow::updateQuery()
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
    queryBuilder.execQuery();
}
void MainWindow::on_updateSettingsButton_clicked()
{
    begin = ui->fromDateEdit->date();
    end = ui->toDateEdit->date();
    if(begin > end)
    {
        QMessageBox::warning(this,tr("NewsGui"),tr("Begin date must be prior to end date! Please change and hit update again."), QMessageBox::Ok);
        return;
    }

    ui->newsListWidget->clear();
    updateQuery();
    QSqlQuery query = queryBuilder.getFinalQuery();

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

//if a category is checked, add it to the active categories list if a category is unchecked remove
//it from active categories list.
void MainWindow::on_categoryListWidget_itemChanged(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked && !activeCategories.contains(item->text()))
            activeCategories.append(item->text());
        else if(item->checkState() == Qt::Unchecked)
            activeCategories.removeAt(activeCategories.indexOf(item->text()));
}

//if a source is checked, add it to the active sources list if a source is unchecked remove
//it from active sources list.
void MainWindow::on_sourcesListWidget_itemChanged(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked && !activeSources.contains(item->text()))
            activeSources.append(item->text());
        else if(item->checkState() == Qt::Unchecked)
            activeSources.removeAt(activeSources.indexOf(item->text()));
}

//when return is pressed on the filter line button then the string that was in the filter line and
//parses it into a string list based on commas. so if bitcoin,economy,stocks is in the line edit when
//return is pressed the the string list would be {bitcoin, economy, stocks}
void MainWindow::on_filterLineEdit_returnPressed()
{
    QString filterText = ui->filterLineEdit->text();
    filterList = parseList(filterText);
    on_updateSettingsButton_clicked();
}

//function is used to search source list, checks to see if a source has a partial match
//with the string passed as a parameter. If theres atleast one match then it is selected
//otherwise jump back to the top of the sources list.
void MainWindow::on_sourcesLineEdit_textEdited(const QString &arg1)
{
    QRegExp searchTerm(".*" + arg1 + ".*");//regex for partial string match
    //create a vector of widget pointers to the sources that satisfy the regex
    QList<QListWidgetItem *> widgets = ui->sourcesListWidget->findItems(searchTerm.pattern(), Qt::MatchRegExp);
    //as long as theres atleast one match set the first match as the selected item and put it at the top of the scrollbox
    if(widgets.size() > 0)
    {
        ui->sourcesListWidget->setCurrentItem(widgets[0]);
        ui->sourcesListWidget->scrollToItem(widgets[0],QAbstractItemView::PositionAtTop);
    }
    //otherwise display an error and jump to the top of the list
    else
    {
        qDebug() << "couldn't find " << arg1;
        ui->sourcesListWidget->setCurrentRow(0);
    }
}

//function just handles whether the date should be incorporated in the query
// if it changes state and is in the on state then the date will be included in the query
//otherwise it wont be.
void MainWindow::on_dateCheckBox_stateChanged(int arg1)
{
    if(ui->dateCheckBox->checkState() == Qt::Checked)
        limitDate = true;
    else
        limitDate = false;

}

void MainWindow::on_rppComboBox_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            pageCount = 10;
            break;
        case 1:
            pageCount = 25;
            break;
        case 2:
            pageCount = 50;
            break;
        case 3:
            pageCount = 100;
            break;
        default:
            qDebug() << "Qt broke.";
            break;

    }
}
