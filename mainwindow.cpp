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
#include <cstdlib>
#include <ctime>
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
    user = db.getUsuarios();

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
    ui->RB_admin->setVisible(false);
    mostrarUsuarios();


    // Agregar insumos al combo box

    actualizarCBES();

    db.inicializarUsuarios();


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
        if(db.accederLogin(user, password, username)) {
            ID=db.existeUsuario(user,username);
            ui->RB_admin->setVisible(ID >= 500 ? true : false);
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
    ui->tab_copeco->setCurrentIndex(0);
    vector<Insumo> insumos = db.getAllInsumos();


    ui->tw_Mostrar->setRowCount(insumos.size());
    ui->tw_Mostrar->setColumnCount(5);

    QStringList headers;
    headers << "Codigo" << "Descripción" << "Entradas" << "Salidas" << "Saldo Actual";
    ui->tw_Mostrar->setHorizontalHeaderLabels(headers);
    ui->tw_Mostrar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    // Llena la tablewidget
    for (int row = 0; row <insumos.size();row++) {
        ui->tw_Mostrar->setItem(row, 0, new QTableWidgetItem(QString::number(insumos[row].getId())));
        ui->tw_Mostrar->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(insumos[row].getDescripcion())));
        ui->tw_Mostrar->setItem(row, 2, new QTableWidgetItem(QString::number(insumos[row].getentradas())));
        ui->tw_Mostrar->setItem(row, 3, new QTableWidgetItem(QString::number(insumos[row].getsalidas())));
        ui->tw_Mostrar->setItem(row, 4, new QTableWidgetItem(QString::number(insumos[row].getsaldoActual())));
    }
}


void MainWindow::on_rb_verEntradas_clicked() {
    vector<ES> esRegistros = db.getAll_ES();
    vector<Insumo> insumos = db.getAllInsumos();
    ui->tw_Mostrar->setColumnCount(7);
    QStringList headers = {"Codigo", "Descripción", "Fecha", "Cantidad", "Procedencia", "Responsable", "Recibido"};
    ui->tw_Mostrar->setHorizontalHeaderLabels(headers);
    ui->tw_Mostrar->setRowCount(0);
    ui->tw_Mostrar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for (const ES &es : esRegistros) {
        if (!es.getFecha().empty() && es.getFecha()[0] != '-') { // Filtra solo entradas
            QString descripcionInsumo;
            for (const Insumo &insumo : insumos) {
                if (insumo.getId() == es.getInsumo()) {
                    descripcionInsumo = QString::fromStdString(insumo.getDescripcion());
                    break;
                }
            }

            ui->tw_Mostrar->insertRow(row);
            ui->tw_Mostrar->setItem(row, 0, new QTableWidgetItem(QString::number(es.getInsumo())));
            ui->tw_Mostrar->setItem(row, 1, new QTableWidgetItem(descripcionInsumo)); // aquí va la descripción
            ui->tw_Mostrar->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(es.getFecha())));
            ui->tw_Mostrar->setItem(row, 3, new QTableWidgetItem(QString::number(es.getCantidad())));
            ui->tw_Mostrar->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(es.getProcedencia())));
            ui->tw_Mostrar->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(es.getResponsable())));
            ui->tw_Mostrar->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(es.getRecibido())));
            row++;
        }
    }
}




void MainWindow::on_RB_admin_clicked()
{

    QImage menu(":/new/prefix1/menu.png");
    ui->lbl_png->setPixmap(QPixmap::fromImage(menu));
    ui->tab_copeco->setCurrentIndex(3);
    ui->le_IDAdm->setText(QString::number(randNum(5)));
}


void MainWindow::on_btn_admin_clicked()
{
    if(ui->le_usernameAdm->text().isEmpty() || ui->le_contraAdm->text().isEmpty() || (!ui->rb_normalAdm->isChecked() && !ui->rb_adminAdm->isChecked())){
        QMessageBox::warning(this,  "Datos incongruentes","Favor, asegurese de llenar todos los campos");
    }else {
        if(db.existeUsuario(user,ui->le_usernameAdm->text().toStdString())!=-1){
            QMessageBox::warning(this,  "Datos incongruentes","Username no disponible");

        }else{
            db.agregarUsuarios(ui->le_IDAdm->text().toUInt(),ui->le_usernameAdm->text().toStdString(),ui->le_contraAdm->text().toStdString() );
            mostrarUsuarios();
            ui->le_usernameAdm->clear();
            ui->le_contraAdm->clear();
            ui->le_IDAdm->clear();
        }
    }

}
void MainWindow::mostrarUsuarios()
{
    user = db.getUsuarios();
    ui->tw_usuarios->clear();
    ui->tw_usuarios->setColumnCount(5);
    QStringList headers = {"Nombre", "ID", "Contraseña", "Cargo","Cambiar Contra"};

    ui->tw_usuarios->setHorizontalHeaderLabels(headers);
    ui->tw_usuarios->setRowCount(user.size());
    ui->tw_usuarios->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int row = 0; row < user.size(); row++) {
        int id = user[row].getId();
        ui->tw_usuarios->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(user[row].getNombre())));
        ui->tw_usuarios->setItem(row, 1, new QTableWidgetItem(QString::number(id)));
        ui->tw_usuarios->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(user[row].getContrasena())));

        QString txt = (id >= 500) ? "ADMINISTRADOR" : "NORMAL";
        ui->tw_usuarios->setItem(row, 3, new QTableWidgetItem(txt));
        ui->tw_usuarios->setItem(row, 4, new QTableWidgetItem("CAMBIAR"));
    }
}
int MainWindow::randNum(short num)
{
    user= db.getUsuarios();
    int nuevoID;
    do{
        if(num == 5){
            nuevoID= rand() % 400 + 100;
        }else{
            nuevoID= rand() % 500 + 500;
        }
    }while(existeID(nuevoID));

    return nuevoID;
}

bool MainWindow::existeID(int id)
{
    for(auto& usuario: user) {
        if (usuario.getId()==id) {
            return true;
        }
    }
    return false;
}

void MainWindow::on_tw_usuarios_cellClicked(int row, int column)
{
    if(column==4){
        user = db.getUsuarios();

        bool ok;
        QString neuePass = QInputDialog::getText(this, "Nueva Contraseña", "Ingrese la nueva contraseña:", QLineEdit::Normal, "", &ok);
        if (ok && !neuePass.isEmpty()) {
            for ( Usuarios& usuario : user) {
                QString idTab = ui->tw_usuarios->item(row, 1)->text();
                int idsele = idTab.toInt();

                if (usuario.getId()== idsele) {
                    usuario.setContrasena(neuePass.toStdString());

                    if (!db.actualizarUsuario(usuario)) {
                        QMessageBox::information(this, "Datos no funcan", "No se ha podido actualizar la base de datos");
                    }
                    break;
                }
            }
        }
        else {
            QMessageBox::information(this, "Datos incongruentes", "Favor, no deje campos vacios");
        }
        mostrarUsuarios();
    }

}


void MainWindow::on_rb_normalAdm_clicked()
{
    ui->le_IDAdm->setText(QString::number(randNum(5)));
}


void MainWindow::on_rb_inventario_3_clicked()
{
}


void MainWindow::on_rb_adminAdm_clicked()
{
    ui->le_IDAdm->setText(QString::number(randNum(88)));

}

void MainWindow::on_rb_verInsumo_clicked()
{
    bool ok;
    int insumoId = QInputDialog::getInt(this, "Filtrar por Insumo", "Ingrese el ID del Insumo:", 0, 0, 2147483647, 1, &ok);

    if (ok) {
        vector<ES> esRegistros = db.getAll_ES();
        vector<Insumo> insumos = db.getAllInsumos();
        ui->tw_Mostrar->setColumnCount(7);
        QStringList headers = {"Codigo", "Descripción", "Fecha", "Cantidad", "Procedencia", "Responsable", "Recibido"};
        ui->tw_Mostrar->setHorizontalHeaderLabels(headers);
        ui->tw_Mostrar->setRowCount(0);
        ui->tw_Mostrar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        bool insumoExiste = false;
        bool tieneEntradasOSalidas = false;
        int row = 0;
        QString insumoPaDesc;


        // Verificar si el insumo existe en la tabla Insumos
        for (const Insumo &insumo : insumos) {
            if (insumo.getId() == insumoId) {
                insumoPaDesc = QString::fromStdString(insumo.getDescripcion());
                insumoExiste = true;
                break;
            }
        }

        if (insumoExiste) {
            // Si el insumo existe, buscar sus entradas y salidas
            for (const ES &es : esRegistros) {
                if (es.getInsumo() == insumoId) {
                    tieneEntradasOSalidas = true;

                    ui->tw_Mostrar->insertRow(row);
                    ui->tw_Mostrar->setItem(row, 0, new QTableWidgetItem(QString::number(es.getInsumo())));
                    ui->tw_Mostrar->setItem(row, 1, new QTableWidgetItem(insumoPaDesc));                    ui->tw_Mostrar->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(es.getFecha())));
                    ui->tw_Mostrar->setItem(row, 3, new QTableWidgetItem(QString::number(es.getCantidad())));
                    ui->tw_Mostrar->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(es.getProcedencia())));
                    ui->tw_Mostrar->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(es.getResponsable())));
                    ui->tw_Mostrar->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(es.getRecibido())));
                    row++;
                }
            }

            if (!tieneEntradasOSalidas) {
                QMessageBox::information(this, "Sin Entradas o Salidas", "El insumo con el ID especificado no tiene entradas ni salidas registradas.");
            }
        } else {
            QMessageBox::information(this, "Insumo no existente", "El insumo con el ID especificado no existe.");
        }
    }
}



void MainWindow::on_rb_verSalidas_clicked() {
    vector<ES> esRegistros = db.getAll_ES();
    ui->tw_Mostrar->setColumnCount(6);
    QStringList headers = {"Codigo", "Fecha", "Cantidad", "Procedencia", "Responsable", "Recibido"};
    ui->tw_Mostrar->setHorizontalHeaderLabels(headers);
    ui->tw_Mostrar->setRowCount(0);
    ui->tw_Mostrar->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int row = 0;
    for (const ES &es : esRegistros) {
        if (!es.getFecha().empty() && es.getFecha()[0] == '-') { // Filtra solo salidas
            ui->tw_Mostrar->insertRow(row);
            ui->tw_Mostrar->setItem(row, 0, new QTableWidgetItem(QString::number(es.getInsumo())));
            ui->tw_Mostrar->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(es.getFecha())));
            ui->tw_Mostrar->setItem(row, 2, new QTableWidgetItem(QString::number(es.getCantidad())));
            ui->tw_Mostrar->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(es.getProcedencia())));
            ui->tw_Mostrar->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(es.getResponsable())));
            ui->tw_Mostrar->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(es.getRecibido())));
            row++;
        }
    }
}


