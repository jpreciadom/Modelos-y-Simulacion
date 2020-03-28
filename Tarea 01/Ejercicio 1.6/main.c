#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "lcgrand.h"

#define NUMMAQUINAS 3                           //Van a haber 3 maqinas
#define TIEMPODEUSO 3600                        //Cada maquina se usa 3600 segundos
#define NUMEVENTOS 3

struct Pieza {
    long TiempoDeEntradaALaMaquina;
    long TiempoDeProcesamiento;
};

struct Maquina {
    short Id;                                   //Numero de la máquina
    short EstadoDeActividad;                    //Si esta ocupada o disponible
    long TiempoDeUso;                           //Tiempo total en segundos que ha sido usada la máquina
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
struct Maquina Maquinas[NUMMAQUINAS];
struct Queue *Piezas;
long Eventos[NUMEVENTOS];                       //En el evento 0 llega una pieza nueva a la cola y en el evento 1 termina de ser procesada una pieza
                                                //el evento 3 es el cambio de máquina
//Contadores estadisticos
int PiezasProcesadas;
long TotalTiempoEsperado;
int MaxCantPiezasQueEsperaron;

void Inicializacion(){
    Reloj = 0;
    MaquinaEnUso = 0;
    for(int i = 0; i<NUMMAQUINAS; i++){
        (Maquinas+i)->Id = i + 1;
        (Maquinas+i)->EstadoDeActividad = 0;
        (Maquinas+i)->TiempoDeUso = 0;
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
    Eventos[2] = TIEMPODEUSO;                               //Cambio de maquina en el segundo 0

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
    printf("Inicializando.... ");
    Inicializacion();
    printf("Fin de inicializacion\n\n");
    CambioDeMaquina();
    while(Reloj < tiempoSimulacion){
        printf("Reloj: %li\n", Reloj);
        if(Reloj == Eventos[1])
            PiezaProcesada();
        if(Reloj == Eventos[2])
            CambioDeMaquina();
        if(Reloj == Eventos[0])
            LlegadaDeUnaPieza();
        printf("\n");
        Timing();
    }
    printf("\n\nFin de la simulacion\n\n");

    double promedio = (double)(TotalTiempoEsperado / PiezasProcesadas);
    printf("El promedio esperado por las piezas fue %f\n", promedio);
    for(int i = 0; i<NUMMAQUINAS; i++){
        printf("La maquina %i se uso %li segundos de %i segundos\n", i, Maquinas[i].TiempoDeUso, TIEMPODEUSO);
    }
    printf("%i piezas quedaron sin procesar, mientras que %i piezas fueron procesadas\n", Piezas->Size, PiezasProcesadas);
    printf("El numero maximo de piezas en la cinta fue %i\n", MaxCantPiezasQueEsperaron);
    return 0;
}

double uniform(double a, double b) {                   /* Uniform variate generation function. */
    /* Return a U(a,b) random variate. */
    return a + lcgrand(1) * (b - a);
}

void MeterPiezaALaMaquina(){
    struct Maquina *maq = (Maquinas + MaquinaEnUso);
    long TiempoDisponible = Eventos[2]-Reloj;
    if(Piezas->Size > 0 && maq->EstadoDeActividad == 0 && TiempoDisponible >= maq->RangosDeDemora[1]){
        TotalTiempoEsperado += Reloj - Peek(Piezas)->TiempoDeLlegada;
        printf("Se mete una pieza en la maquina que espero %li segundos\n", Reloj - Peek(Piezas)->TiempoDeLlegada);
        FreeNode(Dequeue(Piezas));
        maq->EstadoDeActividad = 1;
        struct Pieza *p;
        do{
            p = (struct Pieza*)malloc(sizeof(struct Pieza));
        }while(p == NULL);
        maq->TrabajandoEn = p;
        p->TiempoDeEntradaALaMaquina = Reloj;
        p->TiempoDeProcesamiento = uniform(maq->RangosDeDemora[0], maq->RangosDeDemora[1]);
        Eventos[1] = Reloj + p->TiempoDeProcesamiento;
        printf("Sale en %li segundos\n", p->TiempoDeProcesamiento);
     } else if((Piezas->Size == 0 && maq->EstadoDeActividad == 0) || TiempoDisponible < maq->RangosDeDemora[1]){
        Eventos[1] = -1;
     }
}

void CambioDeMaquina(){
    MaquinaEnUso = (long)(Reloj/TIEMPODEUSO);
    printf("Cambio de maquina a la maquina %i\n", MaquinaEnUso);
    MeterPiezaALaMaquina();
    Eventos[2] = (MaquinaEnUso+1)*TIEMPODEUSO;
    printf("Proximo cambio a los %li segundos\n", Eventos[2]);
}

void PiezaProcesada(){
    printf("Se termino de procesar una pieza en la maquina %i\n", MaquinaEnUso);
    PiezasProcesadas++;
    struct Maquina *maq = (Maquinas + MaquinaEnUso);
    maq->TiempoDeUso += maq->TrabajandoEn->TiempoDeProcesamiento;
    maq->PiezasProducidas++;

    maq->EstadoDeActividad = 0;
    maq->TrabajandoEn = NULL;
    MeterPiezaALaMaquina();
}

void LlegadaDeUnaPieza(){
    printf("LLego una pieza\n");
    Enqueue(Piezas, initNode(Reloj));
    MeterPiezaALaMaquina();
    if(MaxCantPiezasQueEsperaron < Piezas->Size)
        MaxCantPiezasQueEsperaron = Piezas->Size;

    Eventos[0] = Reloj + (long)uniform(10, 20);
    printf("La proxima pieza llega a los %li segundos\n", Eventos[0]);
}
