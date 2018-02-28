#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <algorithm>
#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include "querybuilder.h"
#include "sqlconn.hpp"
#include <QDesktopServices>
#include <QListWidget>
#include <QUrl>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    MainWindow(QString,QString,QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_clearSettingsButton_clicked();

    void on_updateSettingsButton_clicked();

    void on_newsListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QSqlDatabase database;
    QueryBuilder queryBuilder;
};

#endif // MAINWINDOW_H
