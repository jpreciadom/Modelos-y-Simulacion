#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "simlib.h"
#include "Lector.h"
#include "LinkedList.h"

//Eventos
#define LLEGADA_DE_UNA_PERSONA                  1
#define CUMPLIMIENTO_DE_TIEMPO_DE_ESPERA        2
#define FIN_DEL_SERVICIO                        3
//Estadisticos
#define ESPERA_EN_LA_COLA                       1
#define ESPERA_EN_EL_SERVIDOR                   2
#define ESPERA_EN_EL_SISTEMA                    3
#define ESPERA_DE_CLIENTES_QUE_RENUNCIAN        4
//Streams
#define STREAM                                  1

struct Cliente {
    float TiempoDeLlegada;
    float LimiteDeEspera;
};

int MinutosDeSimulacion;
int NumSimulaciones;
float ExpoLlegada;
float ExpoServicio;
float Tolerancia[3];
float TiempoDeEspera;
float PoissZonaPermanencia;

FILE *inFile, *outFile;

struct Cliente *clienteSiendoAtendido;
struct LinkedList * queue;
long MaximaCantidadDeClientesEsprando;
long ClientesQueNoEntraron;

void inicializarVariablesDeEntrada();
void inicializarModelo();
void LlegaUnCliente();
void CumplimientoDeTiempoDeEspera();
void PasarAlServidor();
void FinDelServicio();
void report();

int poisson(float media);
int TRIA(int a, int b, int c);

int main() {
    srand(STREAM*71523);
    inicializarVariablesDeEntrada();

    for(int i = 0; i<NumSimulaciones; i++){
        inicializarModelo();

        while(sim_time < MinutosDeSimulacion){
            timing();

            switch(next_event_type){
                case LLEGADA_DE_UNA_PERSONA:
                    LlegaUnCliente();
                    break;
                case CUMPLIMIENTO_DE_TIEMPO_DE_ESPERA:
                    CumplimientoDeTiempoDeEspera();
                    break;
                case FIN_DEL_SERVICIO:
                    FinDelServicio();
                    break;
            }
        }

        report();
    }

    printf("Fin de la simulacion\n");
    return 0;
}

void inicializarVariablesDeEntrada(){
    inFile = fopen("Ejercicio_1.2.in", "r");
    outFile = fopen("Ejercicio_1.2.out", "w+");

    queue = initLinkedList();

    if(inFile == NULL){
        perror("No se pudo abrir el archivo de entrada\n");
        exit(-1);
    } else if(outFile == NULL){
        perror("No se pudo abrir el archivo de salida\n");
        exit(-1);
    } else if(queue == NULL){
        perror("No se pudo crear la cola\n");
        exit(-1);
    }

    init(inFile);
    MinutosDeSimulacion = NextInt();
    NumSimulaciones = NextInt();
    ExpoLlegada = NextFloat();
    ExpoServicio = NextFloat();
    for(int i = 0; i<3; i++)
        Tolerancia[i] = NextFloat();
    TiempoDeEspera = NextFloat();;
    PoissZonaPermanencia = NextFloat();
}

void inicializarModelo(){
    clienteSiendoAtendido = NULL;
    MaximaCantidadDeClientesEsprando = -1;
    MakeEmpty(queue);
    ClientesQueNoEntraron = 0;

    init_simlib();
    event_schedule(sim_time + expon(ExpoLlegada, STREAM), LLEGADA_DE_UNA_PERSONA);
}

void LlegaUnCliente(){
    int Tria = TRIA(Tolerancia[0], Tolerancia[1], Tolerancia[2]);
    if(queue->Size <= Tria){
        struct Cliente *nuevoCliente;
        do{
            nuevoCliente = malloc(sizeof(struct Cliente));
        } while (nuevoCliente == NULL);
        nuevoCliente->TiempoDeLlegada = sim_time;
        nuevoCliente->LimiteDeEspera = sim_time + erlang(2, TiempoDeEspera, STREAM);
        AddLast(queue, nuevoCliente);
        event_schedule(nuevoCliente->LimiteDeEspera, CUMPLIMIENTO_DE_TIEMPO_DE_ESPERA);
        printf("Entra un cliente\n");

        PasarAlServidor();
    } else {
        ClientesQueNoEntraron++;
    }

    event_schedule(sim_time + expon(ExpoLlegada, STREAM), LLEGADA_DE_UNA_PERSONA);
}

void CumplimientoDeTiempoDeEspera(){
    int Pos = -1;
    int poiss = 0;
    float TiempoEsperado;
    GetIterator(queue);
    while(HasNext() == 1){
        struct Cliente *reg = GetNext();
        if (reg->LimiteDeEspera == sim_time){
            Pos = poiss;
            TiempoEsperado = sim_time- reg->TiempoDeLlegada;
            break;
        }
        poiss++;
    }

    poiss = poisson(PoissZonaPermanencia);
    if(Pos > poiss){
        Remove(queue, Pos);
        sampst(TiempoEsperado, ESPERA_DE_CLIENTES_QUE_RENUNCIAN);
    }
}

void PasarAlServidor(){

}

void FinDelServicio(){

}

void report(){
    char * descripciones[5] = { "Espera en la cola",
                                "Espera en el servidor",
                                "Espera en el sistema",
                                "Espera de clientes que renunciaron"};
    for(int i = 0; i<4; i++){
        fprintf(outFile, "Lista N° %i: %s\n", i+1, descripciones[i]);
    }
    out_sampst(outFile, ESPERA_EN_LA_COLA, ESPERA_DE_CLIENTES_QUE_RENUNCIAN);
    fprintf(outFile, "\nTime simulation ended:%12.3f minutos\n\n", sim_time);
}

int poisson(float media){
    return 0;
}

int TRIA(int a, int b, int c){
    return b + (a/c) - (c/a);
}
