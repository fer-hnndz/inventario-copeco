#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include "Insumo.h"
#include "Usuarios.h"
#include <vector>

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
    vector<Usuarios> getUsuarios() {
        QSqlQuery query(conn);
        query.exec("SELECT * FROM insumos;");

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



private:
    /*
     * Crea las tablas para usar la base de datos
     */
    void prepareTables() {
        if (!conn.isOpen()) return;

        QSqlQuery schemaQuery(conn);
        schemaQuery.exec("CREATE TABLE Personas(dni TEXT PRIMARY KEY NOT NULL, nombre string NOT NULL), password TEXT NOT NULL;");
        schemaQuery.exec("CREATE TABLE Insumos(id INTEGER PRIMARY KEY, descripcion TEXT NOT NULL);");
        schemaQuery.exec("CREATE TABLE ES(id INTEGER PRIMARY KEY, insumo INTEGER, fecha INTEGER, cantidad INTEGER, procedencia TEXT, responsable TEXT, recibido TEXT, FOREIGN KEY(responsable) REFERENCES Personas(dni), FOREIGN KEY(recibido) REFERENCES Personas(dni));");
        conn.commit();
    }
};


#endif // DATABASE_H
