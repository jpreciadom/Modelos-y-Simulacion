#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcgrand.h"

#define NUMEVENTOS 3
#define TIEMPODEUSO 60


int next_event_type, num_hours, num_N, num_p_esperando_A[51], num_p_esperando_B[51],
    num_in_q_A, num_in_q_B, bus_status, suma_tiempo_esperado, total_pasajeros, bus_station;

float reloj,mean_arrival_A,mean_arrival_B,time_next_event[5], promedio_esperado;

void initialize();
void timing();
void passenger_arrival_A();
void passenger_arrival_B();
void bus_travel();
void bus_arrive();
void report();
void poisson(float mean);
void normal(float mean);
float expon(float mean);
int poissonRandom(double expectedValue);

void initialize (){
reloj=0;
num_in_q_A=0;
num_in_q_B=0;
bus_status=0;
suma_tiempo_esperado=0;
total_pasajeros=0;
bus_station=1;
mean_arrival_A=9;
mean_arrival_B=5;

time_next_event[1]=reloj+expon(mean_arrival_A);   /*llegada de pasajero a A*/
time_next_event[2]=reloj+expon(mean_arrival_B);   /*llegada de pasajero B*/
time_next_event[3]=1.0e+30;           /* inicio y recorrido del bus*/
time_next_event[4]=1.0e+30;            /*llegada de bus a terminal */
time_next_event[5]=num_hours;         /*finaliza la simulacion*/
}

void timing(){
    int i;
    float minTimeNextEvent = 1.0e+29;
    next_event_type=0;
    for(i = 1; i<=NUMEVENTOS; i++)
        if(time_next_event[i] < minTimeNextEvent){
           minTimeNextEvent = time_next_event[i];
           next_event_type = i;
        }

    reloj = minTimeNextEvent;
}

void passenger_arrival_A(){
    num_in_q_A++;
    num_p_esperando_A[num_in_q_A] = reloj;
    if(num_in_q_A==num_N && bus_status==0){
        bus_travel();
    }
    time_next_event[1]=reloj+expon(mean_arrival_A);
}

void passenger_arrival_B(){
    num_in_q_B++;
    num_p_esperando_B[num_in_q_B] = reloj;
    if(num_in_q_B==num_N && bus_status==0){
        bus_travel();
    }
    time_next_event[2]=reloj+expon(mean_arrival_B);
}

void bus_travel(){
    bus_status=1;
    if(bus_station==1){
        total_pasajeros+=num_in_q_A;
        for(int i=1; i<=num_in_q_A; i++){
            suma_tiempo_esperado+=reloj-num_p_esperando_A[i];
            num_p_esperando_A[i]=-1;
        }
        num_in_q_A=0;
    }else{
        total_pasajeros+=num_in_q_B;
        for(int i=1; i<=num_in_q_B; i++){
            suma_tiempo_esperado+=reloj-num_p_esperando_B[i];
            num_p_esperando_B[i]=-1;
        }
        num_in_q_B=0;
    }
    /*se calcula el tiempo de llegada del bus*/
   time_next_event[1]=reloj+expon(mean_arrival_A);
   time_next_event[2]=reloj+expon(mean_arrival_B);
}

void bus_arrive(){
    if(bus_station==1){
        bus_station=2;
    }else if(bus_station==2){
        bus_station=1;
    }
    bus_status=0;
    time_next_event[4]=1.0e+30;
}


float expon(float mean)  /* Exponential variate generation function. */
{
    /* Return an exponential random variate with mean "mean". */

    return -mean * log(lcgrand(1));
}

void report(){
    promedio_esperado = suma_tiempo_esperado/total_pasajeros;
    printf("%f \n",promedio_esperado);
}

int main(){
    initialize();
    do{
        timing();

        switch(next_event_type){
        case 1:
            passenger_arrival_A();
            break;
        case 2:
            passenger_arrival_B();
            break;
        case 3:
            bus_travel();
            break;
        case 4:
            bus_arrive();
            break;
        case 5:
            report();
            break;
        }

    }while(next_event_type!=5);

    return 0;
}


