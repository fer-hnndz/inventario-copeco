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

    // CIFRADO Y DESCIFRADO

    int Modular(int N, int a, int b);
    vector<int *> de2bi(int x);

    /*
         * Prepara la lista de caracteres para la encriptacion
         */
    void prepararCaracteres() {
        caracteresDisponibles.push_back(' ');
        caracteresDisponibles.push_back('!');
        caracteresDisponibles.push_back('#');
        caracteresDisponibles.push_back('$');
        caracteresDisponibles.push_back(',');
        caracteresDisponibles.push_back('.');

        // Insertar los caracteres del 0 al 9 (estos la suma que se le hace es convertida a char)
        for (int i = 0; i < 10; i++) {
            caracteresDisponibles.push_back('0' + i);
        }

        caracteresDisponibles.push_back(';');

        // Insertar el alfabeto en mayuscula
        for (int i = 0; i < 26; i++) {
            caracteresDisponibles.push_back('A' + i);
        }

        // Insertar el alfabeto en minuscula
        for (int i = 0; i < 26; i++) {
            caracteresDisponibles.push_back('a' + i);
        }

        // Popular caracteresModulados
        for (int i = 0; i< caracteresDisponibles.size(); i++) {
            caracteresModulados.push_back(Modular((int) caracteresDisponibles.at(i), 11413, 3533));
        }

        ordenarVector(caracteresModulados);
    }

    void ordenarVector(vector<int>& vec) {
        int n = vec.size();

        // Bucle externo para iterar a través de todos los elementos del vector
        for (int i = 0; i < n - 1; i++) {
            // Bucle interno para comparar y intercambiar elementos
            for (int j = 0; j < n - i - 1; j++) {
                // Si el elemento actual es mayor que el siguiente elemento, intercambiarlos
                if (vec[j] > vec[j + 1]) {
                    // Intercambiar los elementos
                    int temp = vec[j];
                    vec[j] = vec[j + 1];
                    vec[j + 1] = temp;
                }
            }
        }
    }

    /*
         * Cifra la cadena ingresa en el TextBox
         */
    string cifrar(string texto);
    string descifrar(string texto);

    vector<char> caracteresDisponibles;
    vector<int> caracteresModulados; // Lista de los caracteres disponibles modulados en orden ascendente.


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
