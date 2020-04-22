#include <stdio.h>
#include <stdlib.h>
#include "simlib.h"

#define EVENT_PRODUCE_PIECE        1  /* Event type for proudce a piece. */
#define EVENT_START_TRAVEL     2  /* Event type for start travel . */
#define EVENT_UNLOADING     3  /* Event type for unloading */
#define EVENT_ARRIVE_A      4  /*llegada de montacargas a A*/
#define LIST_MACHINE_A           1  /* List number for machine A. */
#define SAMPST_DELAYS_A        1  /* sampst variable for delays in machine A. */
#define STREAM_PRODUCE_A  1  /* Random-number stream for pieces produce in A. */

int   num_pieces_queue_A, num_N, montacargas_state, tamano;
float mean_produce_A, desviation, mean_intertravel;
FILE  *infile, *outfile;

void init_model(void);
void produce_piece(void);
void start_travel(void);
void unloading(void);
void arrive_montacargas_A(void);
void report(void);
float normal(void);

int main()
{
    srand(time(0));
    infile  = fopen("Ejercicio_2.7.in",  "r");
    outfile = fopen("Ejercicio_2.7.out", "w");

    /* Read input parameters. */
    for(int i=1; i<=10; i++){
    init_simlib();

    /* Set maxatr = max(maximum number of attributes per record, 4) */

    maxatr = 4;  /* NEVER SET maxatr TO BE SMALLER THAN 4. */

    /* Initialize the model. */

    init_model();
    num_N = i;
    fprintf(outfile,"N es igual a %i \n\n",num_N);
    /* Run the simulation while more delays are still needed. */

    while (sim_time < 480) {

        /* Determine the next event. */

        timing();

        /* Invoke the appropriate event function. */

        switch (next_event_type) {
            case EVENT_PRODUCE_PIECE:
                produce_piece();
                break;
            case EVENT_START_TRAVEL:
                start_travel();
                break;
            case EVENT_UNLOADING:
                unloading();
                break;
            case EVENT_ARRIVE_A:
                arrive_montacargas_A();
                break;
        }
    }

    report();
    }
    fclose(infile);
    fclose(outfile);

    return 0;
}

void init_model(void)  /* Initialization function. */
{
    num_pieces_queue_A=0;
    //num_N=5;
    mean_intertravel = 3;
    desviation = 0.08333;
    mean_produce_A = 1.1111;
    montacargas_state = 0;
    event_schedule(sim_time + expon(mean_produce_A, STREAM_PRODUCE_A),
                   EVENT_PRODUCE_PIECE);
}

void produce_piece(void){

    event_schedule(sim_time + expon(mean_produce_A, STREAM_PRODUCE_A),
                   EVENT_PRODUCE_PIECE);

    transfer[1] = sim_time;
    list_file(LAST,LIST_MACHINE_A);

    if(list_size[LIST_MACHINE_A]>=num_N && montacargas_state==0){
        event_schedule(sim_time + (0.16666),EVENT_START_TRAVEL);
        montacargas_state=1;
    }
}

void start_travel(void){
    tamano = list_size[LIST_MACHINE_A];
        for(int i=1; i<=tamano; i++){
            list_remove(FIRST,LIST_MACHINE_A);
            sampst(sim_time - transfer[1], SAMPST_DELAYS_A);
        }
    event_schedule(sim_time + normal(),EVENT_UNLOADING);
}

void unloading(void){
    event_schedule(sim_time + normal() + (0.16666),EVENT_ARRIVE_A);
}

void arrive_montacargas_A(void){
    montacargas_state=0;
    if(list_size[LIST_MACHINE_A]>=num_N && montacargas_state==0){
        event_schedule(sim_time + (0.16666),EVENT_START_TRAVEL);
        montacargas_state=1;
    }
}

void report(void){
    fprintf(outfile, "\nDelays in queue, in minutes:\n");
    out_sampst(outfile, SAMPST_DELAYS_A, SAMPST_DELAYS_A);
    /*
    fprintf(outfile, "\nFila maquina A (1)):\n");
    out_filest(outfile, LIST_MACHINE_A,LIST_MACHINE_A);
    */
    fprintf(outfile, "\nTime simulation ended:%12.3f minutes\n\n", sim_time);
}



float normal(){
    float aux=0, cK=1000;
    for (int k=1;k<=cK;k++){
        aux=aux+(float)rand()/RAND_MAX;
    }
    return desviation*sqrt((float)12/cK)*(aux-(float)cK/2)+mean_intertravel;
}

