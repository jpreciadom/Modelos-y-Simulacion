#include <stdio.h>
#include <stdlib.h>
#include "simlib.h"

#define EVENT_ARRIVE_C    1  /* Event type for proudce a piece. */
#define EVENT_ENTER_ATM1  2  /* Event type for start travel . */
#define EVENT_ENTER_ATM2  3  /* Event type for start travel . */
#define EVENT_ENTER_ATM3  4  /* Event type for start travel . */
#define EVENT_ENTER_ATM4  5  /* Event type for start travel . */
#define EVENT_ENTER_ATM5  6  /* Event type for start travel . */
#define EVENT_ENTER_ATM6  7  /* Event type for start travel . */
#define EVENT_EXIT_ATM1   8  /* Event type for unloading */
#define EVENT_EXIT_ATM2   9  /* Event type for unloading */
#define EVENT_EXIT_ATM3   10  /* Event type for unloading */
#define EVENT_EXIT_ATM4   11  /* Event type for unloading */
#define EVENT_EXIT_ATM5   12  /* Event type for unloading */
#define EVENT_EXIT_ATM6   13  /* Event type for unloading */
#define LIST_QUEUE_1      1  /* List number for machine A. */
#define LIST_QUEUE_2      2  /* List number for machine A. */
#define LIST_QUEUE_3      3  /* List number for machine A. */
#define LIST_QUEUE_4      4  /* List number for machine A. */
#define LIST_QUEUE_5      5  /* List number for machine A. */
#define LIST_QUEUE_6      6  /* List number for machine A. */
#define SAMPST_DELAYS_Q1  1  /* sampst variable for delays in machine A. */
#define SAMPST_DELAYS_Q2  2  /* sampst variable for delays in machine A. */
#define SAMPST_DELAYS_Q3  3  /* sampst variable for delays in machine A. */
#define SAMPST_DELAYS_Q4  4  /* sampst variable for delays in machine A. */
#define SAMPST_DELAYS_Q5  5  /* sampst variable for delays in machine A. */
#define SAMPST_DELAYS_Q6  6  /* sampst variable for delays in machine A. */
#define STREAM_ARRIVAL    1
#define STREAM_SERVICE    2  /* Random-number stream for pieces produce in A. */

int categoria_transaccion, num_ATMS, tipo_fila, fila;
float mean_arrivals, mean_service_1, mean_service_2, mean_service_3, mean_service_4, ATMS[7];
FILE  *infile, *outfile;

void init_model(void);
void arrive_customer(void);
void customer_enters_ATM1(void);
void customer_enters_ATM2(void);
void customer_enters_ATM3(void);
void customer_enters_ATM4(void);
void customer_enters_ATM5(void);
void customer_enters_ATM6(void);
void exit_ATM1(void);
void exit_ATM2(void);
void exit_ATM3(void);
void exit_ATM4(void);
void exit_ATM5(void);
void exit_ATM6(void);
void report(void);

int main()
{
    srand(time(0));
    infile  = fopen("Ejercicio_2.6.in",  "r");
    outfile = fopen("Ejercicio_2.6.out", "w");

    /* Read input parameters. */
    init_simlib();

    /* Set maxatr = max(maximum number of attributes per record, 4) */

    maxatr = 4;  /* NEVER SET maxatr TO BE SMALLER THAN 4. */

    /* Initialize the model. */

    init_model();

    /* Run the simulation while more delays are still needed. */

    while (sim_time < 480) {

        /* Determine the next event. */

        timing();

        /* Invoke the appropriate event function. */

        switch (next_event_type) {
            case EVENT_ARRIVE_C:
                arrive_customer();
                break;
            case EVENT_ENTER_ATM1:
                customer_enters_ATM1();
                break;
            case EVENT_ENTER_ATM2:
                customer_enters_ATM2();
                break;
            case EVENT_ENTER_ATM3:
                customer_enters_ATM3();
                break;
            case EVENT_ENTER_ATM4:
                customer_enters_ATM4();
                break;
            case EVENT_ENTER_ATM5:
                customer_enters_ATM5();
                break;
            case EVENT_ENTER_ATM6:
                customer_enters_ATM6();
                break;
            case EVENT_EXIT_ATM1:
                exit_ATM1();
                break;
            case EVENT_EXIT_ATM2:
                exit_ATM2();
                break;
            case EVENT_EXIT_ATM3:
                exit_ATM3();
                break;
            case EVENT_EXIT_ATM4:
                exit_ATM4();
                break;
            case EVENT_EXIT_ATM5:
                exit_ATM5();
                break;
            case EVENT_EXIT_ATM6:
                exit_ATM6();
                break;
        }
    }
    report();
    fclose(infile);
    fclose(outfile);

    return 0;
}

void init_model(void)  /* Initialization function. */
{
    categoria_transaccion = 0;
    mean_arrivals = 0.3333;
    mean_service_1 = 0.75;
    mean_service_2 = 1.25;
    mean_service_3 = 2;
    mean_service_4 = 3;
    tipo_fila = 1;    //cero es unaf ila y uno es 6 filas
    fila = 0;
    for(int i=1; i<=6; i++){
        ATMS[i]=0;
    }


    event_schedule(sim_time + expon(mean_arrivals, STREAM_ARRIVAL),
                   EVENT_ARRIVE_C);
}

void arrive_customer(void){

    //printf("llego cliente en %f \n",sim_time);
    event_schedule(sim_time + expon(mean_arrivals, STREAM_ARRIVAL),
                   EVENT_ARRIVE_C);

    if(tipo_fila==0){
        transfer[1] = sim_time;
        list_file(LAST,LIST_QUEUE_1);

        for(int i=1; i<=6; i++){
            if(ATMS[i]==0){
                event_schedule(sim_time,EVENT_ENTER_ATM1);
                ATMS[i]=1;
                break;
            }
        }

    }else{
        int valor = 10000;
        for(int m=1; m<=6; m++){
            if(list_size[m]<valor){
                valor = list_size[m];
                fila = m;
            }
        }
        transfer[1] = sim_time;
        list_file(LAST,fila);

            switch(fila)
        {
            case 1:
                if(ATMS[1]==0){
                    event_schedule(sim_time,EVENT_ENTER_ATM1);
                    ATMS[1]=1;
                }
                break;

            case 2:
                if(ATMS[2]==0){
                    event_schedule(sim_time,EVENT_ENTER_ATM2);
                    ATMS[2]=1;
                }
                break;

            case 3:
                if(ATMS[3]==0){
                    event_schedule(sim_time,EVENT_ENTER_ATM3);
                    ATMS[3]=1;
                }
                break;

            case 4:
                if(ATMS[4]==0){
                    event_schedule(sim_time,EVENT_ENTER_ATM4);
                    ATMS[4]=1;
                }
                break;

            case 5:
                if(ATMS[5]==0){
                    event_schedule(sim_time,EVENT_ENTER_ATM5);
                    ATMS[5]=1;
                }
                break;

            case 6:
                if(ATMS[6]==0){
                    event_schedule(sim_time,EVENT_ENTER_ATM6);
                    ATMS[6]=1;
                }
                break;
        }

    }

    //printf("num cajero %i \n", fila);

}

void customer_enters_ATM1(void){

    //printf("cliente entra a cajero 1 %f \n",sim_time);

    list_remove(FIRST, LIST_QUEUE_1);
    sampst(sim_time - transfer[1], SAMPST_DELAYS_Q1);

    categoria_transaccion = (rand() % 100)+1;
    if(categoria_transaccion>=1 && categoria_transaccion<=15){
            event_schedule(sim_time+expon(mean_service_1, STREAM_SERVICE),EVENT_EXIT_ATM1);
            //printf("categoria transa 111111111111111111111111111111 \n");
    }else if(categoria_transaccion>15 && categoria_transaccion<=44){
            event_schedule(sim_time+expon(mean_service_2, STREAM_SERVICE),EVENT_EXIT_ATM1);
            //printf("categoria transa 222222222222222222222222222222 \n");
    }else if(categoria_transaccion>44 && categoria_transaccion<=76){
            event_schedule(sim_time+expon(mean_service_3, STREAM_SERVICE),EVENT_EXIT_ATM1);
            //printf("categoria transa 333333333333333333333333333333 \n");
    }else if(categoria_transaccion>76 && categoria_transaccion<=100){
            event_schedule(sim_time+expon(mean_service_4, STREAM_SERVICE),EVENT_EXIT_ATM1);
            //printf("categoria transa 444444444444444444444444444444 \n");
    }
}

void customer_enters_ATM2(void){
    //printf("cliente entra a cajero 2 %f \n",sim_time);

    list_remove(FIRST, LIST_QUEUE_2);
    sampst(sim_time - transfer[1], SAMPST_DELAYS_Q2);

    categoria_transaccion = (rand() % 100)+1;
    if(categoria_transaccion>=1 && categoria_transaccion<=15){
            event_schedule(sim_time+expon(mean_service_1, STREAM_SERVICE),EVENT_EXIT_ATM2);
            //printf("categoria transa 111111111111111111111111111111 \n");
    }else if(categoria_transaccion>15 && categoria_transaccion<=44){
            event_schedule(sim_time+expon(mean_service_2, STREAM_SERVICE),EVENT_EXIT_ATM2);
            //printf("categoria transa 222222222222222222222222222222 \n");
    }else if(categoria_transaccion>44 && categoria_transaccion<=76){
            event_schedule(sim_time+expon(mean_service_3, STREAM_SERVICE),EVENT_EXIT_ATM2);
            //printf("categoria transa 333333333333333333333333333333 \n");
    }else if(categoria_transaccion>76 && categoria_transaccion<=100){
            event_schedule(sim_time+expon(mean_service_4, STREAM_SERVICE),EVENT_EXIT_ATM2);
            //printf("categoria transa 444444444444444444444444444444 \n");
    }
}

void customer_enters_ATM3(void){
    //printf("cliente entra a cajero 3 %f \n",sim_time);

    list_remove(FIRST, LIST_QUEUE_3);
    sampst(sim_time - transfer[1], SAMPST_DELAYS_Q3);

    categoria_transaccion = (rand() % 100)+1;
    if(categoria_transaccion>=1 && categoria_transaccion<=15){
            event_schedule(sim_time+expon(mean_service_1, STREAM_SERVICE),EVENT_EXIT_ATM3);
            //printf("categoria transa 111111111111111111111111111111 \n");
    }else if(categoria_transaccion>15 && categoria_transaccion<=44){
            event_schedule(sim_time+expon(mean_service_2, STREAM_SERVICE),EVENT_EXIT_ATM3);
            //printf("categoria transa 222222222222222222222222222222 \n");
    }else if(categoria_transaccion>44 && categoria_transaccion<=76){
            event_schedule(sim_time+expon(mean_service_3, STREAM_SERVICE),EVENT_EXIT_ATM3);
            //printf("categoria transa 333333333333333333333333333333 \n");
    }else if(categoria_transaccion>76 && categoria_transaccion<=100){
            event_schedule(sim_time+expon(mean_service_4, STREAM_SERVICE),EVENT_EXIT_ATM3);
            //printf("categoria transa 444444444444444444444444444444 \n");
    }
}

void customer_enters_ATM4(void){
    //printf("cliente entra a cajero 4 %f \n",sim_time);

    list_remove(FIRST,LIST_QUEUE_4);
    sampst(sim_time - transfer[1], SAMPST_DELAYS_Q4);

    categoria_transaccion = (rand() % 100)+1;
    if(categoria_transaccion>=1 && categoria_transaccion<=15){
            event_schedule(sim_time+expon(mean_service_1, STREAM_SERVICE),EVENT_EXIT_ATM4);
            //printf("categoria transa 111111111111111111111111111111 \n");
    }else if(categoria_transaccion>15 && categoria_transaccion<=44){
            event_schedule(sim_time+expon(mean_service_2, STREAM_SERVICE),EVENT_EXIT_ATM4);
            //printf("categoria transa 222222222222222222222222222222 \n");
    }else if(categoria_transaccion>44 && categoria_transaccion<=76){
            event_schedule(sim_time+expon(mean_service_3, STREAM_SERVICE),EVENT_EXIT_ATM4);
            //printf("categoria transa 333333333333333333333333333333 \n");
    }else if(categoria_transaccion>76 && categoria_transaccion<=100){
            event_schedule(sim_time+expon(mean_service_4, STREAM_SERVICE),EVENT_EXIT_ATM4);
            //printf("categoria transa 444444444444444444444444444444 \n");
    }
}

void customer_enters_ATM5(void){
    //printf("cliente entra a cajero 5 %f \n",sim_time);

    list_remove(FIRST, LIST_QUEUE_5);
    sampst(sim_time - transfer[1], SAMPST_DELAYS_Q5);

    categoria_transaccion = (rand() % 100)+1;
    if(categoria_transaccion>=1 && categoria_transaccion<=15){
            event_schedule(sim_time+expon(mean_service_1, STREAM_SERVICE),EVENT_EXIT_ATM5);
            //printf("categoria transa 111111111111111111111111111111 \n");
    }else if(categoria_transaccion>15 && categoria_transaccion<=44){
            event_schedule(sim_time+expon(mean_service_2, STREAM_SERVICE),EVENT_EXIT_ATM5);
            //printf("categoria transa 222222222222222222222222222222 \n");
    }else if(categoria_transaccion>44 && categoria_transaccion<=76){
            event_schedule(sim_time+expon(mean_service_3, STREAM_SERVICE),EVENT_EXIT_ATM5);
            //printf("categoria transa 333333333333333333333333333333 \n");
    }else if(categoria_transaccion>76 && categoria_transaccion<=100){
            event_schedule(sim_time+expon(mean_service_4, STREAM_SERVICE),EVENT_EXIT_ATM5);
            //printf("categoria transa 444444444444444444444444444444 \n");
    }
}

void customer_enters_ATM6(void){
    //printf("cliente entra a cajero 6 %f \n",sim_time);

    list_remove(FIRST, LIST_QUEUE_6);
    sampst(sim_time - transfer[1], SAMPST_DELAYS_Q6);

    categoria_transaccion = (rand() % 100)+1;
    if(categoria_transaccion>=1 && categoria_transaccion<=15){
            event_schedule(sim_time+expon(mean_service_1, STREAM_SERVICE),EVENT_EXIT_ATM6);
            //printf("categoria transa 111111111111111111111111111111 \n");
    }else if(categoria_transaccion>15 && categoria_transaccion<=44){
            event_schedule(sim_time+expon(mean_service_2, STREAM_SERVICE),EVENT_EXIT_ATM6);
            //printf("categoria transa 222222222222222222222222222222 \n");
    }else if(categoria_transaccion>44 && categoria_transaccion<=76){
            event_schedule(sim_time+expon(mean_service_3, STREAM_SERVICE),EVENT_EXIT_ATM6);
            //printf("categoria transa 333333333333333333333333333333 \n");
    }else if(categoria_transaccion>76 && categoria_transaccion<=100){
            event_schedule(sim_time+expon(mean_service_4, STREAM_SERVICE),EVENT_EXIT_ATM6);
            //printf("categoria transa 444444444444444444444444444444 \n");
    }
}

void exit_ATM1(void){

    //printf("salio cliente %f \n", sim_time);

    ATMS[1]=0;

    if(list_size[LIST_QUEUE_1]>0){
        event_schedule(sim_time,EVENT_ENTER_ATM1);
        ATMS[1]=1;
    }

}

void exit_ATM2(void){
    ATMS[2]=0;

    if(list_size[LIST_QUEUE_2]>0){
        event_schedule(sim_time,EVENT_ENTER_ATM2);
        ATMS[2]=1;
    }
}

void exit_ATM3(void){
    ATMS[3]=0;

    if(list_size[LIST_QUEUE_3]>0){
        event_schedule(sim_time,EVENT_ENTER_ATM3);
        ATMS[3]=1;
    }
}

void exit_ATM4(void){
    ATMS[4]=0;

    if(list_size[LIST_QUEUE_4]>0){
        event_schedule(sim_time,EVENT_ENTER_ATM4);
        ATMS[4]=1;
    }
}

void exit_ATM5(void){
    ATMS[5]=0;

    if(list_size[LIST_QUEUE_5]>0){
        event_schedule(sim_time,EVENT_ENTER_ATM5);
        ATMS[5]=1;
    }
}

void exit_ATM6(void){
    ATMS[6]=0;

    if(list_size[LIST_QUEUE_6]>0){
        event_schedule(sim_time,EVENT_ENTER_ATM6);
        ATMS[6]=1;
    }
}

void report(void){
    if(tipo_fila==0){
        fprintf(outfile, "\nDelays in queue 1, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q1, SAMPST_DELAYS_Q1);
        /*
        fprintf(outfile, "\nFila maquina A (1)):\n");
        out_filest(outfile, LIST_MACHINE_A,LIST_MACHINE_A);
        */
        fprintf(outfile, "\nTime simulation ended:%12.3f minutes\n\n", sim_time);
    }else{
        fprintf(outfile, "\nDelays in queue 1, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q1, SAMPST_DELAYS_Q1);
        fprintf(outfile, "\nDelays in queue 2, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q2, SAMPST_DELAYS_Q2);
        fprintf(outfile, "\nDelays in queue 3, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q3, SAMPST_DELAYS_Q3);
        fprintf(outfile, "\nDelays in queue 4, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q4, SAMPST_DELAYS_Q4);
        fprintf(outfile, "\nDelays in queue 5, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q5, SAMPST_DELAYS_Q5);
        fprintf(outfile, "\nDelays in queue 6, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q6, SAMPST_DELAYS_Q6);
        /*
        fprintf(outfile, "\nFila maquina A (1)):\n");
        out_filest(outfile, LIST_MACHINE_A,LIST_MACHINE_A);
        */
        fprintf(outfile, "\nTime simulation ended:%12.3f minutes\n\n", sim_time);
    }
}
