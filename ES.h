#ifndef ES_H
#define ES_H
#include <string>

using std::string;

class ES {
private:
    int id;
    int insumo;
    string fecha;
    int cantidad;
    string procedencia;
    string responsable;
    string recibido;

public:
    ES(int id, int insumo, string fecha, int cantidad, string procedencia, string responsable, string recibido) {
        this->id = id;
        this->insumo = insumo;
        this->fecha = fecha;
        this->cantidad = cantidad;
        this->procedencia = procedencia;
        this->responsable = responsable;
        this->recibido = recibido;
    }

    int getId() const { return id; }
    int getInsumo() const { return insumo; }
    string getFecha() const { return fecha; }
    int getCantidad() const { return cantidad; }
    string getProcedencia() const { return procedencia; }
    string getResponsable() const { return responsable; }
    string getRecibido() const { return recibido; }
};

#endif // ES_H
