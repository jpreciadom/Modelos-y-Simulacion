#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "lcgrand.h"

#define NUMMAQUINAS 3                           //Van a haber 3 maqinas
#define TIEMPODEUSO 360                        //Cada maquina se usa 3600 segundos
#define NUMEVENTOS 3

struct Pieza {
    long TiempoDeEntradaALaMaquina;
    long TiempoDeProcesamiento;
};

struct Maquina {
    short Id;                                   //Numero de la máquina
    short EstadoDeActividad;                    //Si esta ocupada o disponible
    long TiempoDeUso;                           //Tiempo total en segundos que ha sido usada la máquina
    long TiempoDeInactividad;                   //Tiempo que la maquina estuvo desocupada
    int PiezasProducidas;                       //Cantidad de piezas que ha producido la máquina en lo que lleva de uso
    short RangosDeDemora[2];                    //Indica el el tiempo minimo y maximo que se puede demorar en una pieza
    struct Pieza *TrabajandoEn;
};

//Funciones
double uniform(double a, double b);
//Eventos
void LlegadaDeUnaPieza();
void PiezaProcesada();
void CambioDeMaquina();

//Estado del sistema
long Reloj;
int MaquinaEnUso;
struct Maquina *Maquinas;
struct Queue * Piezas;
long Eventos[NUMEVENTOS];                       //En el evento 0 llega una pieza nueva a la cola y en el evento 1 termina de ser procesada una pieza
                                                //el evento 3 es el cambio de máquina
//Contadores estadisticos
int PiezasProcesadas;
long TotalTiempoEsperado;
int MaxCantPiezasQueEsperaron;

void Inicializacion(){
    Reloj = 0;
    MaquinaEnUso = 0;
    Maquinas = (struct Maquina*)malloc(sizeof(struct Maquina) * NUMMAQUINAS);
    for(int i = 0; i<NUMMAQUINAS; i++){
        (Maquinas+i)->Id = i + 1;
        (Maquinas+i)->EstadoDeActividad = 0;
        (Maquinas+i)->TiempoDeUso = 0;
        (Maquinas+i)->TiempoDeInactividad = 0;
        (Maquinas+i)->PiezasProducidas = 0;
        (Maquinas+i)->TrabajandoEn = NULL;
    }
    Maquinas->RangosDeDemora[0] = 16;
    Maquinas->RangosDeDemora[1] = 24;
    (Maquinas+1)->RangosDeDemora[0] = 7;
    (Maquinas+1)->RangosDeDemora[1] = 13;
    (Maquinas+2)->RangosDeDemora[0] = 10;
    (Maquinas+2)->RangosDeDemora[1] = 20;
    //La cola con las piezas
    Piezas = initQueue();
    //Inicializacion de eventos
    Eventos[0] = Reloj + (long)uniform(10, 20);             //Se programa la primera llegada de una pieza
    Eventos[1] = -1;                                        //No se programa ninguna pieza terminada
    Eventos[2] = 0;                                         //Cambio de maquina en el segundo 0

    //Contadores estadisticos
    PiezasProcesadas = 0;
    TotalTiempoEsperado = 0;
    MaxCantPiezasQueEsperaron = 0;
}

void Timing(){
    int i;
    double minTimeNextEvent = 1e28;
    for(i = 0; i<NUMEVENTOS; i++)
        if(Eventos[i] >= 0 && Eventos[i] < minTimeNextEvent)
            minTimeNextEvent = Eventos[i];
    Reloj = (long)minTimeNextEvent;
}

int main() {
    long tiempoSimulacion = NUMMAQUINAS * TIEMPODEUSO;
    Inicializacion();
    while(Reloj < tiempoSimulacion){
        if(Eventos[2] == Reloj){
            CambioDeMaquina();
        }
        if(Eventos[1] == Reloj){
            PiezaProcesada();
        }
        if(Eventos[0] == Reloj){
            LlegadaDeUnaPieza();
        }
        Timing();
    }
    for(int i = 0; i<NUMMAQUINAS; i++){
        printf("Maquina %i:\n", i);
        printf("Tiempo de uso: %li\n", (Maquinas+i)->TiempoDeUso);
        printf("Tiempo inactiva: %li\n\n", (Maquinas+i)->TiempoDeInactividad);
    }
    return 0;
}

double uniform(double a, double b) {                   /* Uniform variate generation function. */
    /* Return a U(a,b) random variate. */
    return a + lcgrand(1) * (b - a);
}

void MeterPiezaALaMaquina(){
    if(Piezas->Size>0){
        TotalTiempoEsperado += Reloj - Peek(Piezas)->TiempoDeLlegada;
        FreeNode(Dequeue(Piezas));
    }
    struct Pieza *p;
    do{
        p = (struct Pieza*)malloc(sizeof(struct Pieza));
    } while(p == NULL);
    p->TiempoDeEntradaALaMaquina = Reloj;
    p->TiempoDeProcesamiento = (long)uniform((Maquinas + MaquinaEnUso)->RangosDeDemora[0], (Maquinas + MaquinaEnUso)->RangosDeDemora[1]);
    (Maquinas + MaquinaEnUso)->TrabajandoEn = p;
    (Maquinas + MaquinaEnUso)->EstadoDeActividad = 1;
    Eventos[1] = p->TiempoDeEntradaALaMaquina + p->TiempoDeProcesamiento;
}

void CambioDeMaquina(){
    MaquinaEnUso = Reloj/TIEMPODEUSO;
    Eventos[2] = (MaquinaEnUso+1)*TIEMPODEUSO;
    if(Piezas->Size > 0)
        MeterPiezaALaMaquina();
}

void PiezaProcesada(){
    PiezasProcesadas++;
    (Maquinas+MaquinaEnUso)->TiempoDeUso += (Maquinas+MaquinaEnUso)->TrabajandoEn->TiempoDeProcesamiento;
    if(Piezas->Size > 0/* && Eventos[2] - Reloj >= (Maquinas+MaquinaEnUso)->RangosDeDemora[1]*/){
        MeterPiezaALaMaquina();
    } else {
        (Maquinas+MaquinaEnUso)->EstadoDeActividad = 0;
        (Maquinas + MaquinaEnUso)->TrabajandoEn = NULL;
    }
}

void LlegadaDeUnaPieza(){
    if((Maquinas + MaquinaEnUso)->EstadoDeActividad == 0 && Piezas->Size == 0){
        MeterPiezaALaMaquina();
    } else {
        Enqueue(Piezas, initNode(Reloj));
    }
    Eventos[0] = Reloj + (long)uniform(10, 20);
    if(MaxCantPiezasQueEsperaron < Piezas->Size)
        MaxCantPiezasQueEsperaron = Piezas->Size;
}
