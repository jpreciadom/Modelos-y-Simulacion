#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lcgrand.h"


int next_event_type,numeventos,num_q_1,num_q_2,server_status1,server_status2,dat,tipo_a,total_clientes_1,total_clientes_2,sal_s1,sal_s2,prom_fila1, prom_fila2;

float reloj,time_next_event[6],num_minu,lista_tiempo_esperado_f1[51],lista_tiempo_esperado_f2[51],num,time_esperado_f1,time_esperado_f2,tiempo_total_s1,tiempo_total_s2,mean_serv1,mean_serv2;

FILE  *outfile;

void initialize(void);
void timing(void);
void cliente_arrive_s1(void);
void cliente_arrive_s2(void);
void cliente_entra_s1(void);
void cliente_entra_s2(void);
void cliente_sale_s1(void);
void cliente_sale_s2(void);
void report(void);
double uniform(double a, double b);
float expon(float mean);

int main(){
    srand(time(0));

    outfile = fopen("mm1.out", "w");

    numeventos=5;
for(int i=0; i<20; i++){
    initialize();
    do{
        timing();
        switch(next_event_type){
        case 1:
            cliente_arrive_s1();
            break;
        case 2:
            cliente_arrive_s2();
            break;
        case 3:
            cliente_sale_s1();
            break;
        case 4:
            cliente_sale_s2();
            break;
        case 5:
            report();
            break;
        }

    }while(next_event_type!=5);
    printf("\n\n");
}
}

void initialize(){
reloj=0;
num_minu=900.0;
server_status1=0;
server_status2=0;
num_q_1=0;
num_q_2=0;
time_esperado_f1=0;
time_esperado_f2=0;
tiempo_total_s1=0;
tiempo_total_s2=0;
total_clientes_1=0;
total_clientes_2=0;
sal_s1=0;
sal_s2=0;
prom_fila1=0;
prom_fila2=0;

time_next_event[1]=reloj+uniform(1.0,2.0); //llega a filacliente a servidor 1
time_next_event[2]=1.0e+30; //llega acliente fila a servidor 2
time_next_event[3]=1.0e+30; //salida servidor 1
time_next_event[4]=1.0e+30; //salida servidor 2
time_next_event[5]=num_minu; //finaliza la simulacion
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
    //printf("llego cliente a f1 en %f \n",reloj);
    num_q_1++;
    prom_fila1+=num_q_1;
    //printf("fila 1 %i \n",num_q_1);
    total_clientes_1=total_clientes_1+1;
    tipo_a = (rand() % 10)+1;
    if(tipo_a<=3 && num_q_1>0){
        for(int i=num_q_1; i>=1; i--){
            lista_tiempo_esperado_f1[i+1]=lista_tiempo_esperado_f1[i];
        }
        lista_tiempo_esperado_f1[1]=reloj;
    }else{
        lista_tiempo_esperado_f1[num_q_1]=reloj;
    }

    if(server_status1==0){
        cliente_entra_s1();
    }
    time_next_event[1]=reloj+uniform(1.0,2.0);
    //printf("proximo cliente llega a f1 en %f \n",time_next_event[1]);
}

void cliente_arrive_s2(){
    //printf("llego cliente a f2 en %f \n",reloj);
    num_q_2++;
    prom_fila2+=num_q_2;
    //printf("fila 2 %i \n",num_q_2);
    total_clientes_2++;
    lista_tiempo_esperado_f2[num_q_2]=reloj;
    if(server_status2==0){
        cliente_entra_s2();
    }
    time_next_event[2]=1.0e+30;
}

void cliente_entra_s1(){
    //printf("entro cliente a s1 en %f \n",reloj);
    server_status1=1;
    time_esperado_f1+=reloj-lista_tiempo_esperado_f1[1];
    //printf("re %f tiempo esp %f total %f \n",reloj,lista_tiempo_esperado_f1[1], time_esperado_f1);
    num_q_1--;
    //printf("fila 1 %i \n",num_q_1);
    for(int i=1; i<num_q_1; i++){
        lista_tiempo_esperado_f1[i]=lista_tiempo_esperado_f1[i+1];
    }
    time_next_event[3]=reloj+expon(1.0);
    //printf("saldra de s1 en %f \n",time_next_event[3]);
    tiempo_total_s1+=time_next_event[3]-reloj;
}

void cliente_entra_s2(){
    //printf("entro cliente a s2 en %f \n",reloj);
    server_status2=1;
    time_esperado_f2+=reloj-lista_tiempo_esperado_f2[1];
    //printf("re %f tiempo esp %f total %f \n",reloj,lista_tiempo_esperado_f2[1], time_esperado_f2);
    num_q_2--;
    //printf("fila 2 %i \n",num_q_2);
    for(int i=1; i<num_q_2; i++){
        lista_tiempo_esperado_f2[i]=lista_tiempo_esperado_f2[i+1];
    }
    time_next_event[4]=reloj+expon(0.8);
    //printf("saldra de s2 en %f \n",time_next_event[4]);
    tiempo_total_s2+=time_next_event[4]-reloj;
}

void cliente_sale_s1(){
    //printf("sale cliente de s1 en %f \n",reloj);
    if(num_q_1>0){
        cliente_entra_s1();
    }
    server_status1=0;
    time_next_event[2]=reloj+uniform(0.5,2.0);
    //printf("proximo cliente llega a f2 en %f \n",time_next_event[2]);
    time_next_event[3]=1.0e+30;
    sal_s1++;
}

void cliente_sale_s2(){
    //printf("sale cliente de s2 en %f \n",reloj);
    if(num_q_2>0){
        cliente_entra_s2();
    }
    server_status2=0;
    time_next_event[4]=1.0e+30;
    sal_s2++;
}

void report(){
    float promedio1 = (time_esperado_f1/total_clientes_1);
    float promedio2 = (time_esperado_f2/total_clientes_2);
    float promedio_per1 = prom_fila1/total_clientes_1;
    float promedio_per2 = prom_fila2/total_clientes_2;
    float promedio = (promedio1+promedio2)/2;
    float promedio_uso1 = tiempo_total_s1/total_clientes_1;
    float promedio_uso2 = tiempo_total_s2/total_clientes_2;
    printf("promedio de espera en fila 1 %f promedio de espera en fila 2 %f promedio de espera total %f \n",promedio1,promedio2,promedio);
    fprintf(outfile,"promedio de espera en fila 1 %f promedio de espera en fila 2 %f promedio de espera total %f \n",promedio1,promedio2,promedio);
    printf("promedio de personas en fila 1%f promedio de personas en fila 2 %f\n",promedio_per1,promedio_per2);
    fprintf(outfile,"promedio de personas en fila 1%f promedio de personas en fila 2 %f \n",promedio_per1,promedio_per2);
    printf("promedio de uso de servidor 1 %f promedio de uso de servidor 2 %f \n",promedio_uso1,promedio_uso2);
    fprintf(outfile,"promedio de uso de servidor 1 %f promedio de uso de servidor 2 %f \n\n",promedio_uso1,promedio_uso2);
}


float expon(float mean)  /* Exponential variate generation function. */
{
    //int f = rand() % 20;
    return -mean * log((lcgrand(1)));
}

double uniform(double a, double b) {                   /* Uniform variate generation function. */
    dat = rand() % 20;
    for (int i=1; i<dat; i++)
    {
        num=(double)rand()/RAND_MAX*(b-a)+a;
    }
    return num;
}
