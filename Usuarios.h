#ifndef USUARIOS_H
#define USUARIOS_H
#include <string>
using std::string;

class Usuarios{
private:
    int id;
    string nombre;
    string contrasena;

public:
    Usuarios(int id, string nombre,string contrasena) {
        this->id = id;
        this->nombre = nombre;
        this->contrasena= contrasena;
    }

    int getId() { return id; }
    string getNombre() { return nombre; }
    string getContrasena() { return contrasena; }

    /*
     * ACLAREMOS LO SIGUIENTE:
     * los usuarios normalitos tendran un ID mayor a 100 pero menor a 500
     * los usuarios administrados seran >= a 500 y menor a 1000
     * estos ID SE CREARAN DE FORMA ALEATORIA SIEMPRE Y CUANDO NO SEAN IGUALES A LOS EXISTENTES
     * */
    void setId(int newId) { id = newId; }
     void setContrasena(const string& newContrasena) { contrasena = newContrasena; }

};

#endif // USUARIOS_H
