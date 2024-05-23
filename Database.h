#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include "Insumo.h"
#include "Usuarios.h"
#include <vector>
#include <QDateTime>

using std::vector;

class Database {
public:
    QSqlDatabase conn; // Conexion a  la base de datos.

    /*
     * Realiza la conexion inicial a la base de datos.
     */
    bool connect() {
        conn = QSqlDatabase::addDatabase("QSQLITE");
        conn.setDatabaseName("copeco.sql");

        if (!conn.open()) return false;
        prepareTables();
        return true;

    }

    /*
     * Returna un vector de todos los insumos registrados
     */
    vector<Insumo> getAllInsumos() {
        QSqlQuery query(conn);
        query.exec("SELECT * FROM insumos;");

        vector<Insumo> insumos;
        while(query.next()) {
            int id = query.value(0).toInt();
            string descripcion = query.value(1).toString().toStdString();
            Insumo insumo(id, descripcion);

            insumos.push_back(insumo);
        }

        return insumos;

    }

    /*
     * Asigna el insumo con la id especificada al parametro `encontrado`
     * Si no se encuentra ese insumo, el valor de `encontrado` no es modificado.
     */
    void getInsumoById(int id, Insumo &encontrado) {
        vector<Insumo> insumos = getAllInsumos();

        for (Insumo i: insumos) {
            if (i.getId() == id) {
                encontrado = i;
                break;
            }
        }
    }

    /*
     * Retorna un puntero del insumo con la ID especificada.
     * Retorna nullptr si no fue encontrado.
     *
     * NOTA: Este puntero se debe borrar una vez terminado de usarse.
     */
    Insumo* getInsumoById(int id) {
        vector<Insumo> insumos = getAllInsumos();
        for (int i = 0; i < insumos.size(); i++) {

            if (insumos.at(i).getId() == id) {
                Insumo ins = insumos.at(i);
                return new Insumo(ins.getId(), ins.getDescripcion());
            }
        }
        return nullptr;
    }

    vector<Usuarios> getUsuarios() {
        QSqlQuery query(conn);
        query.exec("SELECT * FROM Personas;");

        vector<Usuarios> users;
        while(query.next()) {
            int id = query.value(0).toInt();
            string nombre = query.value(1).toString().toStdString();
            string contrasena = query.value(1).toString().toStdString();
            Usuarios user(id, nombre, contrasena);

            users.push_back(user);
        }

        //usuario de prueba directo
        Usuarios u(455, "OMAR", "123OMAR");
        users.push_back(u);
        Usuarios us(455, "IVAN", "IVANPOTENTE");
        users.push_back(us);
        Usuarios user(455, "VIRGILIO", "VIRGILIOPODEROSO");
        users.push_back(user);


        return users;

    }
    bool existeUsuario(vector<Usuarios> v, string contra, string username){
        for (Usuarios& usuario : v) {
            if (usuario.getNombre() == username) {
                if (usuario.getContrasena() == contra) {
                    return true;
                }
            }
        }
        return false;
    }

    bool agregarInsumos(int codigo, string descripcion) {
        if (!conn.isOpen()) return false;

        QSqlQuery query(conn);
        query.prepare("INSERT INTO insumos (id, descripcion) VALUES (:codigo, :descripcion);");
        query.bindValue(":codigo", codigo);
        query.bindValue(":descripcion", QString::fromStdString(descripcion));

        if (!query.exec()) {
            return false;
        }
        return true;
    }

    //Agrega una entrada a la tabla de entradas
    /*
     *TODO:
     *- Agregar al ui campos de procedencia, nombre del responsable y recibe.
     *- Hacer actualizaciones en el inventario para que se refleje las entradas
     *- Hacer el query de la entrada
     */
    bool registrarEntrada(int cantidad){
        // Registrar fecha y hora y convertirla a string


        if (!conn.isOpen()) {
            return false;
        }else{

            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString date = currentDateTime.date().toString(Qt::ISODate);
            QString time = currentDateTime.time().toString(Qt::ISODate);
            QString fechahora = date + time;

            QSqlQuery query(conn);
            //query.prepare("INSERT INTO ES (fecha, cantidad) VALUES (:id, :fechahora, :cantidad);");
            return true;
        }


        return false;
    }


private:
    /*
     * Crea las tablas para usar la base de datos
     */
    void prepareTables() {
        if (!conn.isOpen()) return;

        QSqlQuery schemaQuery(conn);
        schemaQuery.exec("CREATE TABLE Personas(dni TEXT PRIMARY KEY NOT NULL, nombre string NOT NULL, password TEXT NOT NULL);");
        schemaQuery.exec("CREATE TABLE Insumos(id INTEGER PRIMARY KEY, descripcion TEXT NOT NULL);");
        schemaQuery.exec("CREATE TABLE ES(id INTEGER PRIMARY KEY, insumo INTEGER, fecha TEXT, cantidad INTEGER, procedencia TEXT, responsable TEXT, recibido TEXT, FOREIGN KEY(responsable) REFERENCES Personas(dni), FOREIGN KEY(recibido) REFERENCES Personas(dni));");
        conn.commit();
    }
};


#endif // DATABASE_H
