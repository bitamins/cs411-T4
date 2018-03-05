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
    //create SQLConn object pointer
    SQLConn::Instance();
    //establishes connection
    SQLConn::Instance()->makeConnection(username,pass);
    database = SQLConn::Instance()->getDatabase();
    qDebug()<<"Created db connection.";

    queryBuilder.addDatabase(database);
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
    //get sources list
    QString source = ui->sourcesLineEdit->displayText();
    //get categories list
    QString category = ui->categoriesLineEdit->displayText();
    QStringList categoryList(parseList(category));
    //query the data base for the list of news
    queryBuilder.initQueries(categoryList);
    //update the news list with the new news
    queryBuilder.sort(true);
    queryBuilder.finalizeQueries();
    std::vector<QSqlQuery> queries = queryBuilder.execQueries();
    for(int i =0;i<queries.size();i++)
    {
        while(queries[i].next())
        {
            ui->newsListWidget->addItem("Article");

            QListWidgetItem *articleLink = new QListWidgetItem(ui->newsListWidget);
            articleLink->setTextColor("red");
            articleLink->setData(0, queries[i].value(1).toString());

            ui->newsListWidget->addItem("Description: " + queries[i].value(7).toString());
            ui->newsListWidget->addItem("Source: " + queries[i].value(2).toString());
            ui->newsListWidget->addItem("Picture: " + queries[i].value(5).toString());
            ui->newsListWidget->addItem("Date: " + queries[i].value(6).toString());
            ui->newsListWidget->addItem(" ");
        }
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
    bool _continue = true;
    std::vector<QSqlQuery> queries = queryBuilder.getFinalQueries();

    for(int i = 0; i < queries.size();i++)
    {
        queries[i].seek(-1);
        while(queries[i].next())
        {
            if(queries[i].value(1) == item->text())
            {
                link = queries[i].value(4).toString();
                _continue = false;
                break;

            }
            if(!_continue)
                continue;
        }
    }

      QDesktopServices::openUrl(QUrl(link));

}
