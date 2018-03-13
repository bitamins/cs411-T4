#include "mainwindow.h"
#include "ui_mainwindow.h"

QStringList parseList(QString);
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::MainWindow(QString username, QString pass, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
        ui->setupUi(this);
        //create Settings separate window
        //QWidget* settingsWindow = new QWidget();
        QLayout* settingsGrid = new QGridLayout();
        settingsWindow.setWindowFlags(Qt::Window);
        settingsWindow.setLayout(settingsGrid);
        settingsGrid->addWidget(ui->settingsGroupBox);
        //create SQLConn object pointer
        SQLConn::Instance();
        QStringList columns{"source"};
        //establishes connection
        SQLConn::Instance()->makeConnection(username,pass);
        database = SQLConn::Instance()->getDatabase();
        if(database.isOpen())
            qDebug()<<"Created db connection.";
        else
        {
            qDebug()<<"Failed to connect to database";
            qDebug()<<"Database error " << database.lastError();
        }
        queryBuilder.addDatabase(database);
        queryBuilder.initManual(columns);
        queryBuilder.finalizeQuery();
        QSqlQuery sourcesQuery = queryBuilder.execQuery();
        QStringList sourcesString;
        while(sourcesQuery.next())
            sourcesString.append(sourcesQuery.value(0).toString());

        sourcesQuery.clear();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_clearSettingsButton_clicked()
{
    //clear sources list
    //clear categories list
    ui->sourcesLineEdit->clear();
    ui->categoriesLineEdit->clear();
    ui->newsListWidget->clear();
    queryBuilder.clearQueries();
}



void MainWindow::on_updateSettingsButton_clicked()
{
        queryBuilder.clearQueries();
        ui->newsListWidget->clear();
        //get sources list
        QString source = ui->sourcesLineEdit->displayText();
        //get categories list
        QString category = ui->categoriesLineEdit->displayText();
        QStringList categoryList(parseList(category));
        //query the data base for the list of news
        queryBuilder.initQueries(categoryList);
        //update the news list with the new news
        queryBuilder.sort(true);
        queryBuilder.finalizeQuery();
        QSqlQuery query = queryBuilder.execQuery();
        while(query.next())
        {
            ui->newsListWidget->addItem("Article");

            QListWidgetItem *articleLink = new QListWidgetItem(ui->newsListWidget);
            articleLink->setTextColor("red");
            articleLink->setData(0, query.value(1).toString());
            ui->newsListWidget->addItem("Description: " + query.value(DESCRIPTION).toString());
            ui->newsListWidget->addItem("Source: " + query.value(SOURCE).toString());
            ui->newsListWidget->addItem("Picture: " + query.value(IMAGE).toString());
            ui->newsListWidget->addItem("Date: " + query.value(DATE).toString());
            ui->newsListWidget->addItem("Category: " + query.value(CATEGORY).toString());
            ui->newsListWidget->addItem(" ");
        }

}

QStringList parseList(QString list)
{

    return list.split(",");

}

void MainWindow::on_newsListWidget_itemClicked(QListWidgetItem *item)
{

    QString link;
    qDebug() << item->text() << " clicked";
    QSqlQuery query = queryBuilder.getFinalQuery();
    query.seek(-1);
    while(query.next())
    {
        if(query.value(1) == item->text())
        {
            link = query.value(URL).toString();
            break;
        }

    }

    QDesktopServices::openUrl(QUrl(link));

}

void MainWindow::on_actionSettings_triggered()
{
    settingsWindow.show();
}

void MainWindow::on_lineEdit_returnPressed()
{

}
