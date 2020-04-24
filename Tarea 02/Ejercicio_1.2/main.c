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
#define USO_DEL_SERVIDOR                        2
#define ESPERA_EN_EL_SISTEMA                    3
#define ESPERA_DE_CLIENTES_QUE_RENUNCIAN        4
//Streams
#define STREAM                                  1

struct Cliente {
    float TiempoDeLlegada;
    float LimiteDeEspera;
    float TiempoEnQueIngresoAlServidor;
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

int poisson(float lambda);
int TRIA(int a, int c, int b);

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
        nuevoCliente->TiempoEnQueIngresoAlServidor = -1;
        AddLast(queue, nuevoCliente);
        event_schedule(nuevoCliente->LimiteDeEspera, CUMPLIMIENTO_DE_TIEMPO_DE_ESPERA);

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
    if(queue->Size > 0 && clienteSiendoAtendido == NULL){
        clienteSiendoAtendido = Get(queue, 0);
        clienteSiendoAtendido->TiempoEnQueIngresoAlServidor = sim_time;
        float horaDeLlegada = clienteSiendoAtendido -> TiempoDeLlegada;
        Remove(queue, 0);

        sampst(sim_time - horaDeLlegada, ESPERA_EN_LA_COLA);
        float a = sim_time + expon(ExpoServicio, STREAM);

        printf("Banerita disreta %f - %f\n", a, sim_time);
        event_schedule(a, FIN_DEL_SERVICIO);
        printf("Banerita disreta\n\n");
    }

    if((long)queue->Size > MaximaCantidadDeClientesEsprando){
        MaximaCantidadDeClientesEsprando = queue->Size;
    }
}

void FinDelServicio(){
    sampst(sim_time - clienteSiendoAtendido->TiempoEnQueIngresoAlServidor, USO_DEL_SERVIDOR);
    sampst(sim_time - clienteSiendoAtendido->TiempoDeLlegada, ESPERA_EN_EL_SISTEMA);

    free(clienteSiendoAtendido);
    clienteSiendoAtendido = NULL;
    PasarAlServidor();
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
    fprintf(outFile, "%li no entraron a la fila\n", ClientesQueNoEntraron);
    fprintf(outFile, "La fila alcanzó un maximo de %li clientes\n", MaximaCantidadDeClientesEsprando);
    fprintf(outFile, "Al finalizar la simulación habia %i personas en la fila\n", queue->Size);
    fprintf(outFile, "\nTime simulation ended:%12.3f minutos\n\n", sim_time);
}

int poisson(float lambda){
    float U = uniform(0, 1, STREAM);
    int i = 0;
    float p = exp(-lambda);
    float P = p;
    while(U >= P){
        i++;
        p = lambda * p / i;
        P += p;
    }
    return i;
}

int TRIA(int a, int c, int b){
    float U = uniform(0, 1, STREAM);
    float reg = (float)((float)(c - a) / (float)(b - a));
    if(U < reg){
        U *= (b - a) * (c - a);
        U += pow(a, 2);
        U = sqrt(U);
        return (int)(U + a);
    } else if(U == reg){
        return c;
    } else {
        U = U - reg ;
        U *= (b - a) * (b - c) * -1;
        U += pow(c,2);
        U -= (2 * c * b);
        U += pow(b, 2);
        U = -sqrt(U);
        return U + b;
    }
}
