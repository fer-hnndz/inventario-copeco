#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Database.h"
#include <iostream>

using std::cout;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Database db;
    if(db.connect()) {
        cout << "success\n";
    } else cout << "error\n";
}

MainWindow::~MainWindow()
{
    delete ui;
}

