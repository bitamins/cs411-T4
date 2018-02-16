#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Business_toggled(bool checked)
{
    if(checked == true){
        //query database for business
        std::cout << "get business\n";
    }else{
        //hide business news
        std::cout << "hide business\n";
    }
}

void MainWindow::on_Entertainment_toggled(bool checked)
{
    if(checked == true){
        //query database for business
        std::cout << "get entertainment\n";
    }else{
        //hide business news
        std::cout << "hide entertainment\n";
    }
}
