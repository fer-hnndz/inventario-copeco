#ifndef USUARIOS_H
#define USUARIOS_H
#include <string>
using std::string;

class Usuarios{
private:
    int id;
    string nombre;
    string userCifrado;
    string contraCifrada;

public:
    Usuarios(int id, string nombre, string userCifrado, string contraCifrada) {
        this->id = id;
        this->nombre = nombre;
        this->userCifrado = userCifrado;
        this->contraCifrada = contraCifrada;
    }

    int getId() { return id; }
    string getNombre() { return nombre; }

    /*
     * ACLAREMOS LO SIGUIENTE:
     * los usuarios normalitos tendran un ID mayor a 100 pero menor a 500
     * los usuarios administrados seran >= a 500 y menor a 1000
     * estos ID SE CREARAN DE FORMA ALEATORIA SIEMPRE Y CUANDO NO SEAN IGUALES A LOS EXISTENTES
     * */
    void setId(int newId) { id = newId; }

     const string &getUserCifrado() const;
     void setUserCifrado(const string &newUserCifrado);
     const string &getContraCifrada() const;
     void setContraCifrada(const string &newContraCifrada);
};

inline const string &Usuarios::getContraCifrada() const
{
    return contraCifrada;
}

inline void Usuarios::setContraCifrada(const string &newContraCifrada)
{
    contraCifrada = newContraCifrada;
}

inline const string &Usuarios::getUserCifrado() const
{
    return userCifrado;
}

inline void Usuarios::setUserCifrado(const string &newUserCifrado)
{
    userCifrado = newUserCifrado;
}

#endif // USUARIOS_H
