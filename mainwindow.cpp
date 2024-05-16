#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Database.h"

#include <QtWidgets>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <istream>
#include <iostream>
#include <string>
#include <sstream>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QFileDialog>
#include <QProcess>
#include <fstream>
#include <iostream>
#include <sstream>
using std::ofstream;
using std::ios;
using std::string;
using std::ios;
using std::cout;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_copeco->tabBar()->hide();

    Database db;
    if(db.connect()) {
        cout << "success\n";
    } else cout << "error\n";

    //asignacion imagen y hides iniciales
    QImage menu(":/new/prefix1/menu.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->lbl_1->setStyleSheet("background-color: #000000;");
    ui->lbl_2->setStyleSheet("background-color: #000000;");
    ui->frame->setVisible(false);
    ui->f_acciones->setVisible(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_rb_inventario_clicked()
{
    QImage menu(":/new/prefix1/inv.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->tab_copeco->setCurrentIndex(0);
}


void MainWindow::on_rb_entrada_clicked()
{
    QImage menu(":/new/prefix1/ent.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->tab_copeco->setCurrentIndex(2);

}

//radio buton salida
void MainWindow::on_radioButton_3_clicked()
{
    QImage menu(":/new/prefix1/sal.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->tab_copeco->setCurrentIndex(1);

}


void MainWindow::on_btn_ingresar_clicked()
{
    ui->f_acciones->setVisible(true);
    ui->frame->setVisible(true);
    ui->f_login->setVisible(false);
}

