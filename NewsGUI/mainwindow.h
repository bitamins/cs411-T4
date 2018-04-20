#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <algorithm>
#include <QMainWindow>
#include <QDebug>
#include <QSqlDatabase>
#include <QDesktopServices>
#include <QListWidget>
#include <QUrl>
#include <QLabel>
#include <QDialog>
#include <QSqlError>
#include <QSettings>
#include <QDateTime>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QObject>

#include "pagemanager.h"
#include "querybuilder.h"
#include "sqlconn.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow* Instance(QString username, QString pass);
    void setupCategories();
    void setupSources();
    void driver();
    void restoreSettings();
    void constructQueryWithLimit();
    int getTotalQuerySizeBeforeLimit();
    void goBackAPage(bool isBackwards);
    void goToPageEntered();
    int extractPageNum();

    int startRow;
    int rowsPerPage;
    ~MainWindow();


protected:
     explicit MainWindow(QWidget *parent = 0);
     MainWindow(QString,QString,QWidget *parent = 0);

private slots:
    void on_clearSettingsButton_clicked();

    void on_updateSettingsButton_clicked();

    void on_newsListWidget_itemClicked(QListWidgetItem *item);

    void on_actionSettings_triggered();

    void on_categoryListWidget_itemChanged(QListWidgetItem *item);

    void on_sourcesListWidget_itemChanged(QListWidgetItem *item);

    void on_filterLineEdit_returnPressed();

    void on_sourcesLineEdit_textEdited(const QString &arg1);

    void on_dateCheckBox_stateChanged();

    void on_NextPage_clicked();

    void on_GoBack_clicked();

    void on_pageNum_returnPressed();

private:
    Ui::MainWindow *ui;
    static MainWindow* _instance;

    QDialog settingsWindow;
    QSqlDatabase database;
    QueryBuilder queryBuilder;

    QSettings settings;
    QDate begin;
    QDate end;

    QStringList sources;
    QStringList activeSources;
    QStringList activeCategories;
    QStringList filterList;

    bool limitDate;
    bool test;

    int querySize;
    int currentPage;

};

#endif // MAINWINDOW_H
