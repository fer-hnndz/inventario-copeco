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

    // Deshabilitar componentes que son para mostrar info

    ui->le_descripcion->setDisabled(true); // descripcion de entradas
    ui->sp_saldo->setDisabled(true);
    ui->sp_entradas_salidas->setDisabled(true);


    // Agregar insumos al combo box

    actualizarCBES();

    db.crearUsuarios();
    user = db.getUsuarios();

    ui->tab_copeco->setCurrentIndex(0);
    ui->rb_inventario->setChecked(true);
    //    for(Usuarios& user: user) {
    //       // QString item = QString(".").arg(QString::number(user.getId())).arg(QString::fromStdString(user.getNombre())).arg(QString::fromStdString(user.getContrasena()));

    //    }
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
    ui->tw_Mostrar->setRowCount(0);
    ui->tw_Mostrar->setColumnCount(0);
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
    ui->label->setText("Grupo Destino:");
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
    bool exitos=false, entrada=false;
    int codigoE = ui->cb_codigo->currentText().toInt();
    int cantidadE = ui->sp_cant->value();
    string descripcionE = ui->le_descripcion->text().toStdString();
    string procedencia = ui->le_procedencia->text().toStdString();
    string responsable = ui->le_responsable->text().toStdString();
    string recibe = ui->le_recibido->text().toStdString();
    if(ui->sp_cant->text().toUInt()==0 || ui->le_procedencia->text().isEmpty() || ui->le_responsable->text().isEmpty() || ui->le_recibido->text().isEmpty()){
        QMessageBox::warning(this,  "Datos incongruentes","Favor, asegurese de llenar todos los campos");

        /*
         * CUANDO SE QUIERE HACER ENTRADA
         */
    }else if (ui->rb_entrada->isChecked()){
        if (db.registrarEntrada(codigoE,cantidadE,procedencia,responsable,recibe)){
            QMessageBox::information(this,  "Datos congruentes","Datos han sido ingresados con éxito.");
            exitos=true;
            entrada=true;

        }else{
            QMessageBox::information(this,  "Datos incongruentes","No se ha podido generar la entrada.");
        }
    }else if (ui->RB_Recibir->isChecked()){
        if (db.recibir(codigoE,cantidadE,procedencia,responsable,recibe)){
            QMessageBox::information(this,  "Datos congruentes","Producto ha sido recibido con éxito.");
            exitos=true;
            entrada=true;
        }else{
            QMessageBox::information(this,  "Datos incongruentes","No se ha podido recibir el producto.");
        }
    }else if(ui->radioButton_3->isChecked()){
        int proceso=db.salidaPosible(codigoE,cantidadE,procedencia,responsable,recibe);
        if (proceso !=-1){
            QMessageBox::information(this, "Datos congruentes", "Producto ha sido retirado con éxito.\nInsumo:  " + QString::number(codigoE)+ "\nSaldo actual restante:  "+QString::number(proceso));
            exitos=true;
            entrada=false;
        }else{
            QMessageBox::information(this,  "Datos incongruentes","No se ha podido retirar el producto.\nAsegurese que exista suficiente en saldo actual.");
        }
    }
    if(exitos){
        //  ui->cb_codigo->setCurrentIndex(0); ESTO CRASHEA
        ui->le_descripcion->clear();
        ui->sp_cant->clear();
        ui->sp_entradas_salidas->clear();
        ui->sp_saldo->clear();
        ui->le_procedencia->clear();
        ui->le_responsable->clear();
        ui->le_recibido->clear();

        vector<Insumo> insumos = db.getAllInsumos();
        for ( Insumo& insumo : insumos) {
            if (insumo.getId()==codigoE) {
                //llamar funcion para que se actualice la tabla insumo
                if(entrada){
                    insumo.setentradas(insumo.getentradas()+cantidadE);
                    insumo.setsaldoActual(insumo.getsaldoActual()+cantidadE);
                }else{
                    insumo.setsaldoActual(insumo.getsaldoActual()-cantidadE);
                    insumo.setsalidas(insumo.getsalidas()+cantidadE);
                }

                if (!db.actualizarInsumo(insumo)) {
                    QMessageBox::information(this,  "Datos incongruentes","No se ha podido actualizar DB");
                }
                break;
            }
        }
        exitos=false;
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
        //int codigo=id
        //string descripcion
        // Verificar si el código ya existe
        short code = ui->le_codigoA->text().toShort();
        bool exitente = false;
        vector<Insumo> insumos = db.getAllInsumos();
        for ( Insumo& insumo : insumos) {
            if (insumo.getId()==code) {
                exitente = true;
                break;
            }
        }

        if (!exitente) {
            db.agregarInsumos(code, ui->le_descripcionA->text().toStdString());
            QMessageBox::information(this,  "Datos congruentes","Nuevo insumo ha sido registrado.");
            insumos.clear();
            ui->cb_codigo->blockSignals(true);  // Bloquea señales
            ui->cb_codigo->clear();  // Limpia el ComboBox
            ui->cb_codigo->blockSignals(false);
            actualizarCBES();
            db.getAllInsumos();
            ui->le_codigoA->clear();
            ui->le_descripcionA->clear();
        } else {
            QMessageBox::warning(this, "Datos incongruentes", "El codigo ya existe para otro insumo");
        }


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


void MainWindow::on_cb_codigo_currentTextChanged(const QString &arg1)
{
    int id = arg1.toInt();

    /*
    Insumo i(0, "a"); // crear un insumo vacio
    db.getInsumoById(id, i);
    */

    Insumo *i = db.getInsumoById(id);


    if (i->getDescripcion() == "a")
        ui->le_descripcion->clear();
    else
        ui->le_descripcion->setText(QString::fromStdString(i->getDescripcion()));

    delete i;
}

//DESPUES DE AQUI METAN EL CODIGO PENSANTE

void MainWindow::actualizarCBES() {
    vector<Insumo> insumos = db.getAllInsumos();
    qDebug() << "Añadiendo ítems al ComboBox";
    for(const Insumo& ins: insumos) {
        QString item = QString("%1").arg(QString::number(ins.getId()));
        ui->cb_codigo->addItem(item);
    }
}

void MainWindow::on_RB_Recibir_clicked()
{
    ui->lbl_entradas->setText("Recibidos:");
    ui->tab_copeco->setCurrentIndex(1);

}


void MainWindow::on_rb_verResumen_clicked()
{
    ui->tab_copeco->setCurrentIndex(3);
    vector<Insumo> insumos = db.getAllInsumos();


    ui->tw_Mostrar->setRowCount(insumos.size());
    ui->tw_Mostrar->setColumnCount(5);

    QStringList headers;
    headers << "Codigo" << "Descripción" << "Entradas" << "Salidas" << "Saldo Actual";
    ui->tw_Mostrar->setHorizontalHeaderLabels(headers);

    // Llena la tablewidget
    for (int row = 0; row <insumos.size();row++) {
        ui->tw_Mostrar->setItem(row, 0, new QTableWidgetItem(QString::number(insumos[row].getId())));
        ui->tw_Mostrar->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(insumos[row].getDescripcion())));
        ui->tw_Mostrar->setItem(row, 2, new QTableWidgetItem(QString::number(insumos[row].getentradas())));
        ui->tw_Mostrar->setItem(row, 3, new QTableWidgetItem(QString::number(insumos[row].getsalidas())));
        ui->tw_Mostrar->setItem(row, 4, new QTableWidgetItem(QString::number(insumos[row].getsaldoActual())));
    }

}


void MainWindow::on_rb_verEntradas_clicked()
{
    vector<ES> esRegistros = db.getAll_ES();

       ui->tw_Mostrar->setColumnCount(7);
       QStringList headers = {"ID", "Insumo", "Fecha", "Cantidad", "Procedencia", "Responsable", "Recibido"};
       ui->tw_Mostrar->setHorizontalHeaderLabels(headers);
       ui->tw_Mostrar->setRowCount(esRegistros.size());

       // Rellenar la tabla con los datos
       for (int row = 0; row <esRegistros.size(); row++) {
           ui->tw_Mostrar->setItem(row, 0, new QTableWidgetItem(QString::number(esRegistros[row].getId())));
           ui->tw_Mostrar->setItem(row, 1, new QTableWidgetItem(QString::number(esRegistros[row].getInsumo())));
           ui->tw_Mostrar->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(esRegistros[row].getFecha())));
           ui->tw_Mostrar->setItem(row, 3, new QTableWidgetItem(QString::number(esRegistros[row].getCantidad())));
           ui->tw_Mostrar->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(esRegistros[row].getProcedencia())));
           ui->tw_Mostrar->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(esRegistros[row].getResponsable())));
           ui->tw_Mostrar->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(esRegistros[row].getRecibido())));
       }
}

