#include <stdio.h>
#include <stdlib.h>
#include "simlib.h"

#define EVENT_ARRIVE_C    1  /* Event type for proudce a piece. */
#define EVENT_ENTER_ATM  2  /* Event type for start travel . */
#define EVENT_EXIT_ATM   8  /* Event type for unloading */
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

int categoria_transaccion, num_ATMS, tipo_fila, fila, num_p;
float mean_arrivals, mean_service_1, mean_service_2, mean_service_3, mean_service_4, ATMS[7];
FILE  *infile, *outfile;

void init_model(void);
void arrive_customer(void);
void customer_enters_ATM(void);
void exit_ATM(void);
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
            case EVENT_ENTER_ATM:
                customer_enters_ATM();
                break;
            case EVENT_EXIT_ATM:
                exit_ATM();
                break;
        }
    }
    //printf("%i \n", num_p);
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
    num_p = 0;
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
    num_p++;
    if(tipo_fila==0){
        for(int i=1; i<=6; i++){
           if(ATMS[i]==0){
                transfer[1] = sim_time;
                transfer[3] = i;
                list_file(LAST,LIST_QUEUE_1);
                event_schedule(sim_time,EVENT_ENTER_ATM);
                ATMS[i]=1;
                break;
           }

           if(i==6){
                transfer[1] = sim_time;
                list_file(LAST,LIST_QUEUE_1);
           }
        }

    }else{

        int valor = 10000;
        for(int i=1; i<=6; i++){
                if(ATMS[i]==0){
                    transfer[1] = sim_time;
                    transfer[3] = i;
                    if(i==1){
                        list_file(LAST,LIST_QUEUE_1);
                    }else if(i==2){
                        list_file(LAST,LIST_QUEUE_2);
                    }else if(i==3){
                        list_file(LAST,LIST_QUEUE_3);
                    }else if(i==4){
                        list_file(LAST,LIST_QUEUE_4);
                    }else if(i==5){
                        list_file(LAST,LIST_QUEUE_5);
                    }else if(i==6){
                        list_file(LAST,LIST_QUEUE_6);
                    }
                    event_schedule(sim_time,EVENT_ENTER_ATM);
                    ATMS[i]=1;
                    break;
                }

                if(list_size[i]<valor){
                    valor = list_size[i];
                    fila = i;
                }

                if(i==6){
                    transfer[1] = sim_time;
                    list_file(LAST,fila);
                }

        }

    //printf("num cajero %i \n", fila);
    }
}


void customer_enters_ATM(void){


    int f = transfer[3];

    //printf("cliente entra a cajero %i \n",f);

    if(tipo_fila==0){
        list_remove(FIRST, LIST_QUEUE_1);
        sampst(sim_time - transfer[1], SAMPST_DELAYS_Q1);
    }else{
        if(f==1){
            list_remove(FIRST, LIST_QUEUE_1);
            sampst(sim_time - transfer[1], SAMPST_DELAYS_Q1);
        }else if(f==2){
            list_remove(FIRST, LIST_QUEUE_2);
            sampst(sim_time - transfer[1], SAMPST_DELAYS_Q2);
        }else if(f==3){
            list_remove(FIRST, LIST_QUEUE_3);
            sampst(sim_time - transfer[1], SAMPST_DELAYS_Q3);
        }else if(f==4){
            list_remove(FIRST, LIST_QUEUE_4);
            sampst(sim_time - transfer[1], SAMPST_DELAYS_Q4);
        }else if(f==5){
            list_remove(FIRST, LIST_QUEUE_5);
            sampst(sim_time - transfer[1], SAMPST_DELAYS_Q5);
        }else if(f==6){
            list_remove(FIRST, LIST_QUEUE_6);
            sampst(sim_time - transfer[1], SAMPST_DELAYS_Q6);
        }
    }


    categoria_transaccion = (rand() % 100)+1;
    printf("categoria %i \n", categoria_transaccion);
    float pru;
    if(categoria_transaccion>=1 && categoria_transaccion<=15){
            transfer[4] = f;
            event_schedule(sim_time+expon(mean_service_1, STREAM_SERVICE),EVENT_EXIT_ATM);
            //printf("categoria transa 111111111111111111111111111111 \n");
    }else if(categoria_transaccion>15 && categoria_transaccion<=44){
            transfer[4] = f;
            pru = expon(mean_service_2, STREAM_SERVICE);
            event_schedule(sim_time+pru,EVENT_EXIT_ATM);
            printf("categoria transa 222222222222222222222222222222 %f \n", pru);
    }else if(categoria_transaccion>44 && categoria_transaccion<=76){
            transfer[4] = f;
            pru = expon(mean_service_3, STREAM_SERVICE);
            event_schedule(sim_time+pru,EVENT_EXIT_ATM);
            printf("categoria transa 333333333333333333333333333333 %f \n",pru);
    }else if(categoria_transaccion>76 && categoria_transaccion<=100){
            transfer[4] = f;
            event_schedule(sim_time+expon(mean_service_4, STREAM_SERVICE),EVENT_EXIT_ATM);
            //printf("categoria transa 444444444444444444444444444444 \n");
    }

}

void exit_ATM(void){

    int j = transfer[4];

    ATMS[j]=0;

    if(tipo_fila==0){
            if(list_size[LIST_QUEUE_1]>0){
                transfer[3] = j;
                event_schedule(sim_time,EVENT_ENTER_ATM);
                ATMS[j]=1;
            }
    }else{
            if(j==1 && list_size[LIST_QUEUE_1]>0){
            transfer[3] = 1;
            event_schedule(sim_time,EVENT_ENTER_ATM);
            ATMS[1]=1;
        }else if(j==2 && list_size[LIST_QUEUE_2]>0){
            transfer[3] = 2;
            event_schedule(sim_time,EVENT_ENTER_ATM);
            ATMS[2]=1;
        }else if(j==3 && list_size[LIST_QUEUE_3]>0){
            transfer[3] = 3;
            event_schedule(sim_time,EVENT_ENTER_ATM);
            ATMS[3]=1;
        }else if(j==4 && list_size[LIST_QUEUE_4]>0){
            transfer[3] = 4;
            event_schedule(sim_time,EVENT_ENTER_ATM);
            ATMS[4]=1;
        }else if(j==5 && list_size[LIST_QUEUE_5]>0){
            transfer[3] = 5;
            event_schedule(sim_time,EVENT_ENTER_ATM);
            ATMS[5]=1;
        }else if(j==6 && list_size[LIST_QUEUE_6]>0){
            transfer[3] = 6;
            event_schedule(sim_time,EVENT_ENTER_ATM);
            ATMS[6]=1;
        }
    }

}


void report(void){
    float promedio = 0;
    if(tipo_fila==0){
        fprintf(outfile, "\nDelays in queue 1, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q1, SAMPST_DELAYS_Q1);
        /*
        fprintf(outfile, "\nFila maquina A (1)):\n");
        out_filest(outfile, LIST_MACHINE_A,LIST_MACHINE_A);
        */
        fprintf(outfile, "\nTime simulation ended:%12.3f minutes\n\n", sim_time);
    }else{
        fprintf(outfile, "\nDelays in queues, in minutes:\n");
        out_sampst(outfile, SAMPST_DELAYS_Q1, SAMPST_DELAYS_Q6);
        sampst(0.0, -LIST_QUEUE_1);
        promedio = promedio + transfer[1];
        sampst(0.0, -LIST_QUEUE_2);
        promedio = promedio + transfer[1];
        sampst(0.0, -LIST_QUEUE_3);
        promedio = promedio + transfer[1];
        sampst(0.0, -LIST_QUEUE_4);
        promedio = promedio + transfer[1];
        sampst(0.0, -LIST_QUEUE_5);
        promedio = promedio + transfer[1];
        sampst(0.0, -LIST_QUEUE_6);
        promedio = promedio + transfer[1];
        promedio = promedio/6;
        /*
        fprintf(outfile, "\nFila maquina A (1)):\n");
        out_filest(outfile, LIST_MACHINE_A,LIST_MACHINE_A);
        */
        fprintf(outfile,"\n el promedio de espera en las colas es: %f\n\n", promedio);
        fprintf(outfile, "\nTime simulation ended:%12.3f minutes\n\n", sim_time);
    }
}
