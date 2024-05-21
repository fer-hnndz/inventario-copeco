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
};

#endif // USUARIOS_H
