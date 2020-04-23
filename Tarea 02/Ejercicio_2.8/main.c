#include <stdio.h>
#include <stdlib.h>
#include "simlib.h"
#include "Lector.h"

//Eventos
#define LLEGADA_DE_UN_BARCO     1
#define FIN_DE_DESCARGA         2
//Listas
#define COLA                    1
//Estadisticos
#define SAMPST_COLA             1
#define SAMPST_GRUAA            2
#define SAMPST_GRUAB            3
#define SAMPST_BARCOS           4
//Streams
#define STREAM_LLEGADA          1

struct BarcoEnDescarga {
    float TiempoDeLlegadaAlPuerto;
    float TiempoDeEntradaALaGrua;
    float TiempoDeFinalizacion;
};

struct Grua {
    long TiempoDeUso;
    struct BarcoEnDescarga *Descargando;
};

int maxSize;
long SegundosSimulacion;

double Expo_Llegada;
double Tiempo_Descarga[2];

struct Grua Gruas[2];

FILE *inFile, *outFile;

void init_Model();
void AgregarACola();
void PasarALasGruas();
void TerminarDescarga();
void report();

int main() {
    inFile = fopen("Ejercicio_2.8.in", "r");
    outFile = fopen("Ejercicio_2.8.out", "w+");

    if(inFile == NULL){
        perror("No se pudo abrir el archivo de datos de entrada\n");
        exit(-1);
    } else if(outFile == NULL){
        perror("No se pudo abrir el archivo de salida de datos\n");
        exit(-1);
    }

    init_simlib();
    init_Model();

    while(sim_time < SegundosSimulacion){
        timing();

        switch(next_event_type){
            case LLEGADA_DE_UN_BARCO:
                AgregarACola();
                break;
            case FIN_DE_DESCARGA:
                TerminarDescarga();
                break;
        }
    }
    report();

    return 0;
}

void init_Model(){
    file = inFile;
    delimiter = ',';

    long Minutos_Dia = 60 * 24;
    Expo_Llegada = 1.25 * Minutos_Dia;
    Tiempo_Descarga[0] = 0.5 * Minutos_Dia;
    Tiempo_Descarga[1] = 1.5 * Minutos_Dia;

    SegundosSimulacion = NextLong() * Minutos_Dia;
    maxSize = -1;

    for(int i = 0; i<2; i++){
        Gruas[i].Descargando = NULL;
        Gruas[i].TiempoDeUso = 0;
    }

    event_schedule(sim_time + expon(Expo_Llegada, STREAM_LLEGADA), LLEGADA_DE_UN_BARCO);
}

void AgregarACola(){
    transfer[1] = sim_time;
    list_file(LAST, COLA);

    if(Gruas[0].Descargando != Gruas[1].Descargando){
        transfer[1] = sim_time;
    } else {
        PasarALasGruas();
    }

    if(list_size[COLA] > maxSize){
        maxSize = list_size[COLA];
    }
    event_schedule(sim_time + expon(Expo_Llegada, STREAM_LLEGADA), LLEGADA_DE_UN_BARCO);
}

void PasarALasGruas(){
    int GruasLibres = 0;
    if(Gruas[0].Descargando == NULL)
        GruasLibres += 1;
    if(Gruas[1].Descargando == NULL)
        GruasLibres += 2;

    if(list_size[COLA] == 0){           //No hay mas barcos en cola, entonces ambas gruas pasar a trabajar a un mismo barco
        if(GruasLibres == 1){
            event_cancel(FIN_DE_DESCARGA);

            struct BarcoEnDescarga *reg = Gruas[1].Descargando;
            reg->TiempoDeFinalizacion = sim_time + ((reg->TiempoDeFinalizacion - sim_time)/2);
            Gruas[0].Descargando = reg;

            event_schedule(reg->TiempoDeFinalizacion, FIN_DE_DESCARGA);
        } else if(GruasLibres == 2){
            event_cancel(FIN_DE_DESCARGA);

            struct BarcoEnDescarga *reg = Gruas[0].Descargando;
            reg->TiempoDeFinalizacion = sim_time + ((reg->TiempoDeFinalizacion - sim_time)/2);
            Gruas[1].Descargando = reg;

            event_schedule(reg->TiempoDeFinalizacion, FIN_DE_DESCARGA);
        }
    } else {
        int n = 0;
        struct BarcoEnDescarga *barcoNuevo = NULL;
        if(GruasLibres == 0){//Ninguna esta libre pero tal vez estan trabajando en el mismo
            if (Gruas[0].Descargando == Gruas[1].Descargando) {//Estan trabajando en el mismo
                event_cancel(FIN_DE_DESCARGA);

                struct BarcoEnDescarga *reg = Gruas[0].Descargando;
                reg->TiempoDeFinalizacion = sim_time + ((reg->TiempoDeFinalizacion-sim_time)*2);
                event_schedule(reg->TiempoDeFinalizacion, FIN_DE_DESCARGA);

                reg = malloc(sizeof(struct BarcoEnDescarga));
                barcoNuevo = reg;
                reg->TiempoDeEntradaALaGrua = sim_time;
                reg->TiempoDeFinalizacion = sim_time + uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA);
                Gruas[1].Descargando = reg;
                event_schedule(reg->TiempoDeFinalizacion, FIN_DE_DESCARGA);
                n = 1;
            }
        } else if(GruasLibres == 1){//la grua 0 esta libre
            struct BarcoEnDescarga *nuevo = malloc(sizeof(struct BarcoEnDescarga));
            barcoNuevo = nuevo;
            nuevo->TiempoDeEntradaALaGrua = sim_time;
            nuevo->TiempoDeFinalizacion = sim_time + uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA);
            Gruas[0].Descargando = nuevo;

            event_schedule(nuevo->TiempoDeFinalizacion, FIN_DE_DESCARGA);
            n = 1;
        } else if(GruasLibres == 2){//la grua 1 esta libre
            struct BarcoEnDescarga *nuevo = malloc(sizeof(struct BarcoEnDescarga));
            barcoNuevo = nuevo;
            nuevo->TiempoDeEntradaALaGrua = sim_time;
            nuevo->TiempoDeFinalizacion = sim_time + uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA);
            Gruas[1].Descargando = nuevo;

            event_schedule(nuevo->TiempoDeFinalizacion, FIN_DE_DESCARGA);
            n = 1;
        } else {//Ambas libres
            struct BarcoEnDescarga *nuevo = malloc(sizeof(struct BarcoEnDescarga));
            barcoNuevo = nuevo;
            nuevo->TiempoDeEntradaALaGrua = sim_time;
            nuevo->TiempoDeFinalizacion = sim_time + (uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA)/2);
            Gruas[0].Descargando = Gruas[1].Descargando = nuevo;

            event_schedule(nuevo->TiempoDeFinalizacion, FIN_DE_DESCARGA);
            n = 1;
        }

        if (n == 1){
            list_remove(FIRST, COLA);
            barcoNuevo->TiempoDeLlegadaAlPuerto = transfer[1];
            sampst(sim_time-transfer[1], SAMPST_COLA);
        }
    }
}

void TerminarDescarga(){
    struct BarcoEnDescarga *reg = Gruas[0].Descargando;

    if(Gruas[0].Descargando == Gruas[1].Descargando && Gruas[0].Descargando != NULL){
        sampst(sim_time - Gruas[0].Descargando->TiempoDeEntradaALaGrua, SAMPST_GRUAA);
        sampst(sim_time - Gruas[1].Descargando->TiempoDeEntradaALaGrua, SAMPST_GRUAB);
        Gruas[0].Descargando = Gruas[1].Descargando = NULL;
    }else if(reg != NULL && reg->TiempoDeFinalizacion == sim_time){
        sampst(sim_time - Gruas[0].Descargando->TiempoDeEntradaALaGrua, SAMPST_GRUAA);
        Gruas[0].Descargando = NULL;
    } else {
        sampst(sim_time - Gruas[1].Descargando->TiempoDeEntradaALaGrua, SAMPST_GRUAB);
        reg = Gruas[1].Descargando;
        Gruas[1].Descargando = NULL;
    }

    sampst(sim_time - reg->TiempoDeLlegadaAlPuerto, SAMPST_BARCOS);
    free(reg);
    PasarALasGruas();
}

void report(){
    char * descripciones[5] = { "Fila de los barcos en el puerto",
                                "Barcos que pasaron por la grua A",
                                "Barcos que pasaron por la grua B",
                                "Barcos que ingresaron al sistema"};
    for(int i = 0; i<4; i++){
        fprintf(outFile, "Lista N° %i: %s\n", i+1, descripciones[i]);
    }
    out_sampst(outFile, SAMPST_COLA, SAMPST_BARCOS);
    fprintf(outFile, "El maximo numero de barcos que esperaron el la cola fue %i\n", maxSize);
    fprintf(outFile, "\nTime simulation ended:%12.3f minutos\n\n", sim_time);
}
