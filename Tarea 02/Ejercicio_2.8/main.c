#include <stdio.h>
#include <stdlib.h>
#include "simlib.h"

//Eventos
#define LLEGADA_DE_UN_BARCO     1
#define FIN_DE_DESCARGA         2
//Listas
#define COLA                    1
//Estadisticos
#define SAMPST_COLA             1
//Streams
#define STREAM_LLEGADA          1

struct BarcoEnDescarga {
    long TiempoDeEntradaALaGrua;
    long TiempoDeFinalizacion;
};

struct Grua {
    long TiempoDeUso;
    struct BarcoEnDescarga *Descargando;
};

long SegundosSimulacion;

double Expo_Llegada;
double Tiempo_Descarga[2];

struct Grua Gruas[2];

FILE *inFile, *outFile;

void init_Model();
void AgregarACola();
void PasarALasGruas();
void TerminarDescarga();

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

    return 0;
}

void init_Model(){
    long Segundos_Dia = 3600 * 24;
    Expo_Llegada = 1.25 * Segundos_Dia;
    Tiempo_Descarga[0] = 0.5 * Segundos_Dia;
    Tiempo_Descarga[1] = 1.5 * Segundos_Dia;

    SegundosSimulacion = 90 * Segundos_Dia;

    for(int i = 0; i<2; i++){
        Gruas[i].Descargando = NULL;
        Gruas[i].TiempoDeUso = 0;
    }

    event_schedule(sim_time + expon(Expo_Llegada, STREAM_LLEGADA), LLEGADA_DE_UN_BARCO);
}

void AgregarACola(){
    list_file(LAST, COLA);

    if(Gruas[0].Descargando != Gruas[1].Descargando)
        transfer[1] = sim_time;
    else
        PasarALasGruas();

    //Planifico la siguiente llegada
}

void PasarALasGruas(){
    int GruasLibres = 0;
    if(Gruas[0].Descargando == NULL)
        GruasLibres += 1;
    if(Gruas[1].Descargando == NULL)
        GruasLibres += 2;

    if(list_size[COLA] == 0){           //No hay mas barcos en cola
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
        if(GruasLibres == 0){//Ninguna esta libre pero tal vez estan trabajando en el mismo
            if (Gruas[0].Descargando == Gruas[1].Descargando) {//Estan trabajando en el mismo
                event_cancel(FIN_DE_DESCARGA);

                struct BarcoEnDescarga *reg = Gruas[0].Descargando;
                reg->TiempoDeFinalizacion = sim_time + ((reg->TiempoDeFinalizacion-sim_time)*2);
                event_schedule(reg->TiempoDeFinalizacion, FIN_DE_DESCARGA);

                reg = malloc(sizeof(struct BarcoEnDescarga));
                reg->TiempoDeEntradaALaGrua = sim_time;
                reg->TiempoDeFinalizacion = sim_time + uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA);
                event_schedule(reg->TiempoDeFinalizacion, FIN_DE_DESCARGA);
            }
        } else if(GruasLibres == 1){//la grua 0 esta libre
            struct BarcoEnDescarga *nuevo = malloc(sizeof(struct BarcoEnDescarga));
            nuevo->TiempoDeEntradaALaGrua = sim_time;
            nuevo->TiempoDeFinalizacion = sim_time + uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA);
            Gruas[0].Descargando = nuevo;

            event_schedule(nuevo->TiempoDeFinalizacion, FIN_DE_DESCARGA);
        } else if(GruasLibres == 2){//la grua 1 esta libre
            struct BarcoEnDescarga *nuevo = malloc(sizeof(struct BarcoEnDescarga));
            nuevo->TiempoDeEntradaALaGrua = sim_time;
            nuevo->TiempoDeFinalizacion = sim_time + uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA);
            Gruas[1].Descargando = nuevo;

            event_schedule(nuevo->TiempoDeFinalizacion, FIN_DE_DESCARGA);
        } else {//Ambas libres
            struct BarcoEnDescarga *nuevo = malloc(sizeof(struct BarcoEnDescarga));
            nuevo->TiempoDeEntradaALaGrua = sim_time;
            nuevo->TiempoDeFinalizacion = sim_time + (uniform(Tiempo_Descarga[0], Tiempo_Descarga[1], STREAM_LLEGADA)/2);
            Gruas[0].Descargando = Gruas[1].Descargando = nuevo;

            event_schedule(nuevo->TiempoDeFinalizacion, FIN_DE_DESCARGA);
        }
    }
}

void TerminarDescarga(){
    struct BarcoEnDescarga *reg = Gruas[0].Descargando;

    if(Gruas[0].Descargando == Gruas[1].Descargando && Gruas[0].Descargando != NULL){
        Gruas[0].Descargando = Gruas[1].Descargando = NULL;
    }else if(reg != NULL && reg->TiempoDeFinalizacion == sim_time){
        Gruas[0].Descargando = NULL;
    } else {
        reg = Gruas[1].Descargando;
        Gruas[1].Descargando = NULL;
    }

    free(reg);
    PasarALasGruas();
}
