#ifndef ES_H
#define ES_H
#include <string>

using std::string;
class ES {
private:
    int id;//pasar a short
    int insumo;
    string fecha;
    int cantidad;
    string procedencia;
    string responsable;
    string recibido;





public:
    ES(int id, int insumo,string fecha, int cantidad, string procedencia, string responsable,string recibido) {
        this->id = id;
        this->insumo=insumo;
        this->fecha= fecha;
        this->cantidad=cantidad;
        this->procedencia=procedencia;
        this->responsable=responsable;
        this->recibido=recibido;
    }

    int getId() const { return id; }
     int getInsumo() { return insumo; }
    string getFecha() const { return fecha; }
    int getCantidad() { return cantidad; }
    string getProcedencia() {return procedencia;}
    string getResponsable() { return responsable; }
    string getRecibido() {return recibido; }



    short hayInsumoSuficiente(int code){


    }

};





#endif // ES_H
