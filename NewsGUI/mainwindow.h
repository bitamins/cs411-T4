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
#include <QSqlError>
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

    void on_actionSettings_triggered();

    void on_lineEdit_returnPressed();

private:
    Ui::MainWindow *ui;
    QWidget settingsWindow;
    QSqlDatabase database;
    QueryBuilder queryBuilder;
};

#endif // MAINWINDOW_H
