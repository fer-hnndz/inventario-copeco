#ifndef INSUMO_H
#define INSUMO_H
#include <string>

using std::string;

class Insumo {
private:
    int id;
    string descripcion;

public:
    Insumo(int id, string descripcion) {
        this->id = id;
        this->descripcion = descripcion;
    }

    int getId() { return id; }
    string getDescripcion() { return descripcion; }
};

#endif // INSUMO_H
