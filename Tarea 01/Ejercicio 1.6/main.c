#include <stdio.h>
#include <stdlib.h>

#define NUMMAQUINAS 3

struct Maquina;
struct Pieza;

struct Maquina {
    short Id;                                   //Numero de la m�quina
    short EstadoDeActividad;                    //Si esta ocupada o disponible
    long TiempoDeUso;                           //Tiempo total en segundos que ha sido usada la m�quina
    long TiempoDeInactividad;                   //Tiempo que la maquina estuvo desocupada
    int PiezasProducidas;                       //Cantidad de piezas que ha producido la m�quina en lo que lleva de uso
    struct Pieza *TrabajandoEn;                 //Indica la pieza en la que esta trabajando la maquina
};

struct Pieza {
    int Id;                                     //Identificador unico de la pieza
    int TiempoDeLlegada[NUMMAQUINAS];           //Momento en el que la pieza llego a la maquina
    long Tiempoesperado[NUMMAQUINAS];           //Tiempo que espero en la i-esima maquina
    struct Maquina *AtendidoPor;                //Indica la m�quina que la esta atendiendo
};

int main() {
    printf("hello world");
    return 0;
}
