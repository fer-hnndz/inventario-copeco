#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Database.h>
#include <string>
#include <iostream>
using std::string;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_rb_inventario_clicked();

    void on_rb_entrada_clicked();

    void on_radioButton_3_clicked();

    void on_btn_ingresar_clicked();

    void on_btn_ingresar_2_clicked();

    void on_btn_actividades_clicked();

    void on_rb_agregarInsumo_clicked();

    void on_btn_agregarInsumo_clicked();

    void on_cb_codigo_currentTextChanged(const QString &arg1);


    void on_RB_Recibir_clicked();

    void on_rb_verResumen_clicked();

    void on_rb_verEntradas_clicked();

    void on_RB_admin_clicked();

    void on_btn_admin_clicked();

    void on_tw_usuarios_cellClicked(int row, int column);

    void on_rb_normalAdm_clicked();

    void on_rb_inventario_3_clicked();

    void on_rb_adminAdm_clicked();

    void on_rb_verInsumo_clicked();

    void on_rb_verSalidas_clicked();

private:
    Ui::MainWindow *ui;
    Database db;
    bool esNumero(const string&);
    void actualizarCBES();

    //componentes de USUARIOS
    vector<Usuarios> user;
    short ID;
    void mostrarUsuarios();
    int randNum(short);
    bool existeID(int id);
};
#endif // MAINWINDOW_H
