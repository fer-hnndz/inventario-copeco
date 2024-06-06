#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include "Insumo.h"
#include "Usuarios.h"
#include "ES.h"
#include <vector>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>

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
            int actual = query.value(2).toInt();
            int entradas = query.value(3).toInt();
            int salidas = query.value(4).toInt();

            Insumo insumo(id, descripcion,actual,entradas,salidas);

            insumos.push_back(insumo);
        }

        return insumos;

    }

    vector<ES> getAll_ES(){
        QSqlQuery query(conn);
        query.exec("SELECT * FROM ES;");

        vector<ES> eS;
        while(query.next()) {
            int id = query.value(0).toInt();
            int insumo = query.value(1).toInt();
            string fecha = query.value(2).toString().toStdString();
            int cantidad = query.value(3).toInt();
            string procedencia = query.value(4).toString().toStdString();
            string responsable= query.value(5).toString().toStdString();
            string recibido= query.value(6).toString().toStdString();



            ES es(id, insumo,fecha,cantidad,procedencia, responsable, recibido);

            eS.push_back(es);
        }

        return eS;
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
                return new Insumo(ins.getId(), ins.getDescripcion(), ins.getsaldoActual(),ins.getentradas(),ins.getsalidas());
            }
        }
        return nullptr;
    }

    vector<Usuarios> getUsuarios() {
        QSqlQuery query(conn);
        query.exec("SELECT * FROM Personas;");

        vector<Usuarios> users;
        while(query.next()) {
            int dni = query.value(0).toInt();
            string nombre = query.value(1).toString().toStdString();
            string password = query.value(2).toString().toStdString();
            string userCifrado = query.value(3).toString().toStdString();
            string contraCifrada = query.value(4).toString().toStdString();
            Usuarios user(dni, nombre, password,userCifrado, contraCifrada);

            users.push_back(user);
        }
        return users;

    }
    void inicializarUsuarios(){
        QSqlQuery query(conn);
        query.exec("SELECT * FROM Personas;");
        query.next();
        if(query.value(0).toInt()>0){
            return;
        }
        vector<Usuarios> users;
        Usuarios u(789, "OMAR", "123OMAR", "D5xW", "ns#D5xW");
        users.push_back(u);
        Usuarios us(456, "VIRGILIO", "VIRGILIOPODERSO", "doWcoboD", "doWcoboDhDX WID");
        users.push_back(us);
        Usuarios user(350, "IVAN", "IVANPOTENTE", "odxA", "odxAhDq Aq");
        users.push_back(user);

        query.prepare("INSERT INTO Personas (dni, nombre, password, userCifrado, contraCifrada) VALUES (:dni, :nombre, :password, :userCifrado, :contraCifrada);");
        for (Usuarios& user : users) {
            query.bindValue(":dni", user.getId());
            query.bindValue(":nombre", QString::fromStdString(user.getNombre()));
            query.bindValue(":password", QString::fromStdString(user.getContrasena()));
            query.bindValue(":userCifrado", QString::fromStdString(user.getUserCifrado()));
            query.bindValue(":contraCifrada", QString::fromStdString(user.getContraCifrada()));

            qDebug() << "a" << user.getId()<<"b"<<QString::fromStdString(user.getNombre())<<"b"<<QString::fromStdString(user.getContrasena());

            if (!query.exec()) {
                qDebug() << "Error al insertar usuario:" << query.lastError().text();
            }
        }
    }

    void agregarUsuarios(int ID, string username, string contra, string userCifrado, string contraCifrada){
        QSqlQuery query(conn);
        query.exec("SELECT * FROM Personas;");

        Usuarios u(ID, username, contra, userCifrado, contraCifrada);

        query.prepare("INSERT INTO Personas (dni, nombre, password, userCifrado, contraCifrada) VALUES (:dni, :nombre, :password, :userCifrado, :contraCifrada);");

        query.bindValue(":dni", u.getId());
        query.bindValue(":nombre", QString::fromStdString(u.getNombre()));
        query.bindValue(":password", QString::fromStdString(u.getContrasena()));
        query.bindValue(":userCifrado", QString::fromStdString(u.getUserCifrado()));
        query.bindValue(":contraCifrada", QString::fromStdString(u.getContraCifrada()));

        qDebug() << "a" << u.getId()<<"b"<<QString::fromStdString(u.getNombre())<<"b"<<QString::fromStdString(u.getContrasena()) <<"c"<<QString::fromStdString(u.getUserCifrado()) <<"d"<<QString::fromStdString(u.getContraCifrada());

        if (!query.exec()) {
            qDebug() << "Error al insertar usuario:" << query.lastError().text();
        }
    }

    bool actualizarUsuario( Usuarios& user) {//se añade o se quita cantidad
            if (!conn.isOpen()) return false;

            QSqlQuery query(conn);
            query.prepare("UPDATE Personas SET nombre = :nombre, password = :password WHERE dni = :dni;");
            query.bindValue(":nombre", QString::fromStdString(user.getNombre()));
            query.bindValue(":password",QString::fromStdString(user.getContrasena()));
            query.bindValue(":dni", user.getId());

            if (!query.exec()) {
                qDebug() << "Error al ejecutar el query de actualización de insumo:" << query.lastError().text();
                return false;
            }
            qDebug() << "Usuario actualizado correctamente. Nuevo nombre:" <<QString::number(user.getId()) <<QString::fromStdString(user.getNombre()) << "Nueva contraseña:" << QString::fromStdString(user.getContrasena());

            return true;
        }

    bool accederLogin(vector<Usuarios> v, string contraCifrada, string usernameCifrado){
        for (Usuarios& usuario : v) {
            if (usuario.getUserCifrado()== usernameCifrado) {
                if (usuario.getContraCifrada() == contraCifrada) {
                    return true;
                }
            }
        }
        return false;
    }
    //reutulizar para saber que tipo de usuario hay login y tambien para saber si ya existe un usuario con ese nombre
    int existeUsuario(vector<Usuarios> v, string username){
        for (Usuarios& usuario : v) {
            if (usuario.getNombre()== username) {
                return usuario.getId();
            }
        }
        return -1;
    }



    bool agregarInsumos(int codigo, string descripcion) {
        if (!conn.isOpen()) return false;
        int actual=0,entradas=0,salidas=0;
        QSqlQuery query(conn);

        query.prepare("INSERT INTO insumos (id, descripcion, actual, entradas, salidas) VALUES (:codigo, :descripcion, :actual, :entradas, :salidas);");
        query.bindValue(":codigo", codigo);
        query.bindValue(":descripcion", QString::fromStdString(descripcion));
        query.bindValue(":actual", actual);
        query.bindValue(":entradas", entradas);
        query.bindValue(":salidas", salidas);


        if (!query.exec()) {
            return false;
        }
        return true;
    }
    bool actualizarInsumo( Insumo& insumo) {//se añade o se quita cantidad
            if (!conn.isOpen()) return false;

            QSqlQuery query(conn);
            query.prepare("UPDATE Insumos SET actual = :actual, entradas = :entradas, salidas = :salidas WHERE id = :id;");
            query.bindValue(":actual", insumo.getsaldoActual());
            query.bindValue(":entradas", insumo.getentradas());
            query.bindValue(":salidas", insumo.getsalidas());
            query.bindValue(":id", insumo.getId());

            if (!query.exec()) {
                qDebug() << "Error al ejecutar el query de actualización de insumo:" << query.lastError().text();
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
    int salidaPosible(int insumo, int cantidad, std::string procedencia, std::string responsable, std::string recibido) {
        vector<Insumo> insumos = getAllInsumos();
        for (Insumo& insumooo : insumos) {
            if (insumooo.getId() == insumo) {
                int actual = insumooo.getsaldoActual() - cantidad;
                return (insumooo.getsaldoActual() >= cantidad) ? (registrarSalida(insumo,cantidad,procedencia,responsable,recibido), actual) : -1;
                break;
            }
        }
    }

    bool registrarSalida(int insumo, int cantidad, std::string procedencia, std::string responsable, std::string recibido) {
        if (!conn.isOpen()) {
            qDebug() << "Error: La conexión a la base de datos no está abierta.";
            return false;
        }


        // Registrar fecha y hora y convertirla a string
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString fechahora = "-" + currentDateTime.toString("yyyy-MM-dd HH:mm:ss");

        QSqlQuery query(conn);
        query.prepare(
                    "INSERT INTO ES "
                    "(insumo, fecha, cantidad, procedencia, responsable, recibido) "
                    "VALUES (:insumo, :fechahora, :cantidad, :procedencia, :responsable, :recibido);");
        query.bindValue(":insumo", insumo);
        query.bindValue(":fechahora", fechahora);
        query.bindValue(":cantidad", cantidad * -1);
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
        schemaQuery.exec("CREATE TABLE Personas(dni TEXT PRIMARY KEY NOT NULL, nombre string NOT NULL, password TEXT NOT NULL, userCifrado TEXT NOT NULL, contraCifrada TEXT NOT NULL);");
        schemaQuery.exec("CREATE TABLE Insumos(id INTEGER PRIMARY KEY, descripcion TEXT NOT NULL, actual INTEGER, entradas INTEGER, salidas INTEGER);");
        schemaQuery.exec("CREATE TABLE ES(id INTEGER PRIMARY KEY AUTOINCREMENT, insumo INTEGER, fecha TEXT, cantidad INTEGER, procedencia TEXT, responsable TEXT, recibido TEXT, FOREIGN KEY(responsable) REFERENCES Personas(dni), FOREIGN KEY(recibido) REFERENCES Personas(dni));");
        conn.commit();
    }
};


#endif // DATABASE_H
