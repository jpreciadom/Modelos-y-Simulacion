#include <stdio.h>
#include <stdlib.h>

#define NUMMAQUINAS 3                           //Van a haber 3 maqinas
#define TIEMPODEUSO 3600                        //Cada maquina se usa 3600 segundos

struct Maquina;

struct Maquina {
    short Id;                                   //Numero de la máquina
    short EstadoDeActividad;                    //Si esta ocupada o disponible
    long TiempoDeUso;                           //Tiempo total en segundos que ha sido usada la máquina
    long TiempoDeInactividad;                   //Tiempo que la maquina estuvo desocupada
    int PiezasProducidas;                       //Cantidad de piezas que ha producido la máquina en lo que lleva de uso
    short RangosDeDemora[2];                    //Indica el el tiempo minimo y maximo que se puede demorar en una pieza
};

//Estado del sistema
long Reloj;
int MaquinaEnUso;
struct Maquina *Maquinas;
//Cola con las piezas

//Contadores estadisticos
int PiezasProcesadas;
long TotalTiempoEsperado;
int MaxCantPiezasQueEsperaron;

void Inicializcion(){
    Reloj = 0;
    MaquinaEnUso = 0;
    Maquinas = (struct Maquina*)malloc(sizeof(struct Maquina) * NUMMAQUINAS);
    for(int i = 0; i<NUMMAQUINAS; i++){
        (Maquinas+i)->Id = i + 1;
        (Maquinas+i)->EstadoDeActividad = 0;
        (Maquinas+i)->TiempoDeUso = 0;
        (Maquinas+i)->TiempoDeInactividad = 0;
        (Maquinas+i)->PiezasProducidas = 0;
    }
    Maquinas->RangosDeDemora[0] = 16;
    Maquinas->RangosDeDemora[1] = 24;
    (Maquinas+1)->RangosDeDemora[0] = 7;
    (Maquinas+1)->RangosDeDemora[1] = 13;
    (Maquinas+2)->RangosDeDemora[0] = 10;
    (Maquinas+2)->RangosDeDemora[1] = 20;
    //La cola con las piezas

    //Contadores estadisticos
    PiezasProcesadas = 0;
    TotalTiempoEsperado = 0;
    MaxCantPiezasQueEsperaron = 0;
}

int main() {
    Inicializcion();
    printf("hello world");
    return 0;
}
