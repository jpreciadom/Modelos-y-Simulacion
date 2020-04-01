#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

#define TIMESIMULATION 7200

struct Pedido {
    long TiempoTomado;
    long Tiempo_Estimado_Entrega;
};

struct Grupo_Personas {
    int Cantidad_Personas;
    long TiempoDeLlegada;
    long TiempoTomaMesa;
    long Estimado_Pensando_Pedido;
    struct Pedido *pedido;
    long Estimado_Comiendo;
};

/*

0. Fin de la simulacion
1. Termina de comer un grupo de personas. El tiempo que tarda de comer un grupo de personas se distribuye de forma normal com media 31 min y varianza 5 min
2. Llega un grupo de personas. Las personas llegan segun una distribuicion de poisson de 6 cada hora
3. Un grupo de personas hace un pedido. Las personas se demoran pensando que pedido tomar segun una exponencial
4. Se entrega un pedido. La demora de entregar un pedido se distribuye de forma normal con media 5 min y varianza 0.5 min

*/

int Tipo_Evento;
long Eventos[5];

long Reloj;
int Num_Mesas;
struct LinkedList *Personas_Esperando;
struct LinkedList *Personas_Con_Pedido;

int Personas_Atendidas;
long Tiempo_Esperado;

void Inicializar(){
    Reloj = 0;
    Num_Mesas = 20;
    Personas_Esperando = (struct LinkedList *)initLinkedList();
    if(Personas_Esperando == NULL){
        perror("No se pudo crear la cola\n");
        exit(-1);
    }

    Personas_Con_Pedido = (struct LinkedList *)initLinkedList();
    if(Personas_Con_Pedido == NULL){
        perror("No se pudo crear la capacidad para las personas en las mesas\n");
        exit(-1);
    }

    Eventos[0] = TIMESIMULATION;
    Eventos[1] = -1;
    Eventos[2] = 0; //Esto lo tengo que ajustar a que llegan con distribucion de poisson
    Eventos[3] = -1;
    Eventos[4] = -1;

    Personas_Atendidas = 0;
    Tiempo_Esperado = 0;
}

void Timing(){
    long next_event = Eventos[0];
    Tipo_Evento = 0;
    for(int i = 1; i<5; i++){
        if(Eventos[i] > 1 && Eventos[i] < next_event){
            next_event = Eventos[i];
            Tipo_Evento = i;
        }
    }
    Reloj = next_event;
}

void SentarPersonas();

void Fin_Comida();
void NuevoGrupo();
void NuevoPedido();
void EntregaPedido();

int main(){
    int Fin_Simulacion = 0;
    Inicializar();
    while(Fin_Simulacion == 0){
        Timing();
        switch(Tipo_Evento){
            case 0:
                Fin_Simulacion = 1;
                break;
            case 1:
                Fin_Comida();
                break;
            case 2:
                NuevoGrupo();
                break;

            case 3:
                NuevoPedido();
                break;
            case 4:
                EntregaPedido();
                break;
        }
        SentarPersonas();
    }
    return 0;
}

void SentarPersonas(){
    while(Personas_Con_Pedido->Size < Num_Mesas && Personas_Esperando > 0){
        struct Grupo_Personas *ParaTomarPedido = (struct Grupo_Personas*)RemoveFirst(Personas_Esperando);
        Tiempo_Esperado += Reloj - ParaTomarPedido->TiempoDeLlegada;
        ParaTomarPedido->Estimado_Pensando_Pedido = Reloj + rand()%5;
        if(ParaTomarPedido->Estimado_Pensando_Pedido < Eventos[3])
            Eventos[3] = ParaTomarPedido->Estimado_Pensando_Pedido;

        AddLast(Personas_Con_Pedido, ParaTomarPedido);
    }
}

void Fin_Comida(){

}

void NuevoGrupo(){

}

void NuevoPedido(){

}

void EntregaPedido(){

}
