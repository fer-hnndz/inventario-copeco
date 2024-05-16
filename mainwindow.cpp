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
    } else QMessageBox::warning(this,  "Base de datos","Error de conexion");

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


//LOGICA ESTETICA UI
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
    ui->lbl_entradas->setText("Entradas:");
    ui->tab_copeco->setCurrentIndex(1);

}
//este es el radio buton salida de productis
void MainWindow::on_radioButton_3_clicked()
{
    QImage menu(":/new/prefix1/sal.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->lbl_entradas->setText("Salidas:");
    ui->tab_copeco->setCurrentIndex(1);
}

void MainWindow::on_btn_ingresar_clicked()
{
    if(ui->le_username->text().isEmpty() || ui->le_contra->text().isEmpty()){
        QMessageBox::warning(this,  "Datos incongruentes","Favor, asegurese de llenar todos los campos");
    }else{
        ui->f_acciones->setVisible(true);
        ui->frame->setVisible(true);
        ui->f_login->setVisible(false);
    }
}
//este es para cerrar sesion
void MainWindow::on_btn_ingresar_2_clicked()
{
    QImage menu(":/new/prefix1/menu.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->frame->setVisible(false);
    ui->f_login->setVisible(true);
    ui->f_acciones->setVisible(false);
}

void MainWindow::on_btn_actividades_clicked()
{
    if(ui->le_descripcion->text().isEmpty() || ui->cb_codigo->currentIndex()==0 || ui->sp_cant->text().toUInt()==0 || ui->sp_saldo->text().toDouble()==0 || ui->sp_entradas_salidas->text().toUInt()==0){
        QMessageBox::warning(this,  "Datos incongruentes","Favor, asegurese de llenar todos los campos");
    }else{
        QMessageBox::information(this,  "Datos congruentes","Datos han sido ingresados con éxito.");
        ui->le_descripcion->clear();
        ui->cb_codigo->setCurrentIndex(0);
        ui->sp_cant->clear();
        ui->sp_entradas_salidas->clear();
        ui->sp_saldo->clear();
    }
}

//DESPUES DE AQUI METAN EL CODIGO PENSANTE
