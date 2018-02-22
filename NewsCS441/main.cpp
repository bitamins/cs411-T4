#include "mainwindow.h"
#include <QApplication>
#include <QString>
#include <vector>
#include "querybuilder.h"
#include "sqlconn.hpp"
#include <qDebug>
int main(int argc, char *argv[])
{
    //username and pass are retrieved from commandline arguments
    //to add coomandline arguments click Projects on the left of
    //qt creator then click run and under command line arguments
    //put the username first and then a space and then the password
    QString userName = argv[1];
    QString pass = argv[2];

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //This following code chunk is just an example of how to use the SQLConn singleton
    //and how to use the querybuilder

    //creates a pointer to an SQLconn object
    SQLConn::Instance();
    //makes a connection to to the database with specified username and password
    SQLConn::Instance()->makeConnection(userName, pass);
    //an example vector of keywords to look for
    std::vector<QString> filterWords{"Bitcoin", "Economy", "Stock"};
    //creates a querybuilder object with the category Business and the database connection
    queryBuilder q("Business", SQLConn::Instance()->getDatabase());
    //sends filterWords to the query builder object and adds them to the query
    q.addFilterWords(filterWords);
    //sorts the results by date in ascending order(pass false to sort by descending)
    q.sort(true);
    //limits the query response to at most 10 results(must be called last)
    q.limitQuery("10");
    //adds a semi-colon to end the query
    q.finalizeQuery();
    //executes the query and returns the result to QSqlQuery final
    QSqlQuery final = q.execQuery();
    //loops through the query response and displays the title and date in the console
    while(final.next())
    {
        qDebug() <<"Title: " << final.value(1).toString() << " Date: " << final.value(6).toString() << "\n";
    }
    return a.exec();
}
