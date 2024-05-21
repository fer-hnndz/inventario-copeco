#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Database.h"
#include <QtWidgets>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <istream>
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
#include <QDateTime>
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

    QDateTime fecha = QDateTime::currentDateTime();
    QString format = fecha.toString("dd/MM/yyyy");
    ui->lbl_fecha->setText(format);

    // Agregar insumos al combo box

    vector<Insumo> insumos = db.getAllInsumos();
    for(Insumo& ins: insumos) {
        QString item = QString("%1 - %2").arg(QString::number(ins.getId())).arg(QString::fromStdString(ins.getDescripcion()));
        ui->cb_codigo->addItem(item);
    }

    user = db.getUsuarios();
    for(Usuarios& user: user) {
        QString item = QString(".").arg(QString::number(user.getId())).arg(QString::fromStdString(user.getNombre())).arg(QString::fromStdString(user.getContrasena()));

    }
    //no se puede modificar, depende del combobox
    ui->le_descripcion->setEnabled(false);

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
        string username = ui->le_username->text().toStdString();
        string password = ui->le_contra->text().toStdString();
        user = db.getUsuarios();
        if(db.existeUsuario(user, password, username)) {
            ui->f_acciones->setVisible(true);
            ui->frame->setVisible(true);
            ui->f_login->setVisible(false);

        }else{
            QMessageBox::warning(this,  "Datos incongruentes","Usuario o clave incorrecta");
        }

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
    ui->le_contra->clear();
    ui->le_username->clear();
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

void MainWindow::on_rb_agregarInsumo_clicked()
{
    QImage menu(":/new/prefix1/agregar.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->tab_copeco->setCurrentIndex(2);
}
void MainWindow::on_btn_agregarInsumo_clicked()
{
    if(ui->le_codigoA->text().isEmpty() || ui->le_descripcionA->text().isEmpty()){
        QMessageBox::warning(this,  "Datos incongruentes","Favor, asegurese de llenar todos los campos");
    }else if(esNumero(ui->le_codigoA->text().toStdString())){
        QMessageBox::information(this,  "Datos congruentes","Nuevo insumo ha sido registrado.");
        //recordar que se debe inicializar su cantidad, saldo y entradas en 0
        ui->le_codigoA->clear();
        ui->le_descripcionA->clear();
    }

}

bool MainWindow::esNumero(const std::string &tt)
{
    for (char car : tt) {
        if (!std::isdigit(car)) {
            QMessageBox::warning(this,  "Datos incongruentes","Favor, asegurese de que el codigo solo contenga digitos");
            return false;
        }
    }
    return true;
}


//DESPUES DE AQUI METAN EL CODIGO PENSANTE





