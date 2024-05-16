#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

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

private:
    /*
     * Crea las tablas para usar la base de datos
     */
    void prepareTables() {
        if (!conn.isOpen()) return;

        QSqlQuery schemaQuery(conn);
        schemaQuery.exec("CREATE TABLE Personas(dni string PRIMARY KEY NOT NULL, nombre string NOT NULL);");
        schemaQuery.exec("CREATE TABLE Insumos(id int PRIMARY KEY NOT NULL, descripcion string NOT NULL);");
        schemaQuery.exec("CREATE TABLE ES(id int PRIMARY KEY NOT NULL, insumo int, fecha int, cantidad int, procedencia string, responsable string, recibido string, FOREIGN KEY(responsable) REFERENCES Personas(dni), FOREIGN KEY(recibido) REFERENCES Personas(dni));");
        conn.commit();
    }
};


#endif // DATABASE_H
