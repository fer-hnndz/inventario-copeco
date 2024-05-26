#ifndef INSUMO_H
#define INSUMO_H
#include <string>

using std::string;

class Insumo {
private:
    int id;//pasar a short
    string descripcion;
    int saldoActual;//lo que hay actualmente
    int entradas;//lo que ha entrado de ese insumo de SIEMPRE
    int salidas;//lo que ha salido de ese insumo


public:
    Insumo(int id, string descripcion,int saldoActual, int entradas, int salidas) {
        this->id = id;
        this->descripcion = descripcion;
        this->saldoActual=saldoActual;
        this->entradas=entradas;
        this->salidas=salidas;
    }

    int getId() { return id; }
    string getDescripcion() { return descripcion; }

    int getsaldoActual() { return saldoActual; }
    int getentradas() { return entradas; }
    int getsalidas() { return salidas; }

    void setsaldoActual(int saldoActual) { this->saldoActual=saldoActual; }
    void setentradas(int entradas) { this->entradas=entradas; }
    void setsalidas(int salidas) { this->salidas=salidas; }

    short hayInsumoSuficiente(int code){


    }

};

#endif // INSUMO_H
