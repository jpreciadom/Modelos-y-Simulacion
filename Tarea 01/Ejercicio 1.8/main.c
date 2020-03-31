#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcgrand.h"


int next_event_type,numeventos,num_minu,num_q_1,num_q_2,server_status1,server_status2,dat,tipo_a;

float reloj,time_next_event[8],lista_tiempo_esperado_f1[51],lista_tiempo_esperado_f2[51],num,time_esperado_s1,time_esperado_s2;

void cliente_arrive_s1(void);
void cliente_arrive_s2(void);
void cliente_entra_s1(void);
void cliente_entra_s2(void);
void cliente_sale_s1(void);
void cliente_sale_s2(void);
double uniform(double a, double b);
float expon(float mean);

int main(){
    srand(time(0));

    initialize();
    do{
        timing();
        switch(next_event_type){
        case 1:

            break;
        case 2:

            break;
        case 3:

            break;
        case 4:

            break;
        case 5:
            report();
            break;
        }

    }while(next_event_type!=5);

}

void initialize(){
reloj=0;
num_minu=900;
server_status1=0;
server_status2=0;
num_q_1=0;
num_q_2=0;
time_esperado_s1=0;
time_esperado_s2=0;

time_next_event[1]=reloj+uniform(1,2); //llega cliente a servidor 1
time_next_event[2]=1.0e+30; //llega acliente a servidor 2
time_next_event[3]=1.0e+30; //entra al servidor 1
time_next_event[4]=1.0e+30; //entra al servidor 2
time_next_event[5]=1.0e+30; //salida servidor 1
time_next_event[6]=1.0e+30; //salida servidor 2
time_next_event[7]=num_minu; //finaliza la simulacion
}

void timing(){
    int i;
    float minTimeNextEvent = 1.0e+29;
    next_event_type=0;
    for(i = 1; i<=numeventos; i++)
        if(time_next_event[i] < minTimeNextEvent){
           minTimeNextEvent = time_next_event[i];
           next_event_type = i;
        }

    reloj = minTimeNextEvent;
}

void cliente_arrive_s1(){
    num_q_1++;
    int pro = (rand() % 10)+1;
    if(pro>3 && num_q_1>0){
        for(int i=1; i<=num_q_1; i++){
            lista_tiempo_esperado_f1[i+1]=lista_tiempo_esperado_f1[i];
        }
        lista_tiempo_esperado_f1[1]=reloj;
    }else{
        lista_tiempo_esperado_f1[num_q_1]=reloj;
    }

    if(server_status1=0){
        cliente_entra_s1();
    }
    time_s1=time_next_event[3]-reloj;
}

void cliente_arrive_s2(){
    num_q_2++;
    lista_tiempo_esperado_f2[num_q_2]=reloj;


}

void cliente_entra_s1(){
    time_esperado_s1=reloj-lista_tiempo_esperado_f1[1];
    for(int i=1; i<num_q_1; i++){
        lista_tiempo_esperado_f1[i]=lista_tiempo_esperado_f1[i+1];
    }

    time_next_event[5]=reloj+expon(1);
}

void cliente_entra_s2(){


}

void cliente_sale_s1(){
    time_next_event[2]=reloj+uniform(0.5,2);
}

void cliente_sale_s2(){

}

void report(){

}


float expon(float mean)  /* Exponential variate generation function. */
{
    int f = rand() % 20;
    return -mean * log((lcgrand(f)));
}

double uniform(double a, double b) {                   /* Uniform variate generation function. */
    dat = rand() % 20;
    for (int i=1; i<dat; i++)
    {
        num=(double)rand()/RAND_MAX*(b-a)+a;
    }
    return num;
}
