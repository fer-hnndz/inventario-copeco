#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include "Insumo.h"
#include "Usuarios.h"
#include <vector>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>

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
     *- Hacer actualizaciones en el inventario para que se refleje las entradas
     */
    bool registrarEntrada(int insumo, int cantidad, std::string procedencia, std::string responsable, std::string recibido) {
        if (!conn.isOpen()) {
            qDebug() << "Error: La conexión a la base de datos no está abierta.";
            return false;
        }

        // Registrar fecha y hora y convertirla a string
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString fechahora=currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

        QSqlQuery query(conn);
        query.prepare(
                    "INSERT INTO ES "
                    "(insumo, fecha, cantidad, procedencia, responsable, recibido) "
                    "VALUES (:insumo, :fechahora, :cantidad, :procedencia, :responsable, :recibido);");
        query.bindValue(":insumo", insumo);
        query.bindValue(":fechahora", fechahora);
        query.bindValue(":cantidad", cantidad);
        query.bindValue(":procedencia", QString::fromStdString(procedencia));
        query.bindValue(":responsable", QString::fromStdString(responsable));
        query.bindValue(":recibido", QString::fromStdString(recibido));

        qDebug() << "Datos a insertar: insumo =" << insumo << ", fecha =" << fechahora << ", cantidad =" << cantidad
                 << ", procedencia =" << QString::fromStdString(procedencia) << ", responsable =" << QString::fromStdString(responsable)
                 << ", recibido =" << QString::fromStdString(recibido);

        if (!query.exec()) {
            qDebug() << "Error al ejecutar el query de entrada:" << query.lastError().text();
            return false;
        }
        return true;
    }

    bool recibir(int insumo, int cantidad, std::string procedencia, std::string responsable, std::string recibido) {
        // Verificar si ya existe la entrada
        QSqlQuery checkQuery(conn);
        checkQuery.prepare("SELECT id FROM ES WHERE insumo = :insumo AND procedencia = :procedencia AND responsable = :responsable AND recibido = :recibido;");
        checkQuery.bindValue(":insumo", insumo);
        checkQuery.bindValue(":procedencia", QString::fromStdString(procedencia));
        checkQuery.bindValue(":responsable", QString::fromStdString(responsable));
        checkQuery.bindValue(":recibido", QString::fromStdString(recibido));
        if (!checkQuery.exec()) {
            qDebug() << "Error al ejecutar la consulta de verificación:" << checkQuery.lastError().text();
            return false;
        }

        // Si no existe la entrada no se agrega el producto
        if (!checkQuery.next()) {
            qDebug() << "Error: No se encontró la entrada correspondiente.";
            return false;
        }

        // Puede ser recibido
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString fechahora = currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

        QSqlQuery insertQuery(conn);
        insertQuery.prepare(
            "INSERT INTO ES "
            "(insumo, fecha, cantidad, procedencia, responsable, recibido) "
            "VALUES (:insumo, :fechahora, :cantidad, :procedencia, :responsable, :recibido);");
        insertQuery.bindValue(":insumo", insumo);
        insertQuery.bindValue(":fechahora", fechahora);
        insertQuery.bindValue(":cantidad", cantidad);
        insertQuery.bindValue(":procedencia", QString::fromStdString(procedencia));
        insertQuery.bindValue(":responsable", QString::fromStdString(responsable));
        insertQuery.bindValue(":recibido", QString::fromStdString(recibido));

        if (!insertQuery.exec()) {
            qDebug() << "Error al ejecutar el query de recepción:" << insertQuery.lastError().text();
            return false;
        }

        return true;
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
        schemaQuery.exec("CREATE TABLE ES(id INTEGER PRIMARY KEY AUTOINCREMENT, insumo INTEGER, fecha TEXT, cantidad INTEGER, procedencia TEXT, responsable TEXT, recibido TEXT, FOREIGN KEY(responsable) REFERENCES Personas(dni), FOREIGN KEY(recibido) REFERENCES Personas(dni));");
        conn.commit();
    }
};


#endif // DATABASE_H
