#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setCentralWidget(ui->MAIN_WIDGET);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Business_toggled(bool checked)
{
    if(checked == true){
        //query database for business
        qDebug() << "get business\n";
    }else{
        //hide business news
        qDebug() << "hide business\n";
    }
}

void MainWindow::on_Entertainment_toggled(bool checked)
{
    if(checked == true){
        //query database for business
        qDebug() << "get entertainment\n";
    }else{
        //hide business news
        qDebug() << "hide entertainment\n";
    }
}
