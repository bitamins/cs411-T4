#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <qsqldatabase.h>
#include "querybuilder.h"
#include "sqlconn.hpp"

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

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
};

#endif // MAINWINDOW_H
