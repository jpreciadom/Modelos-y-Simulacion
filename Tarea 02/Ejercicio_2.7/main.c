#include <stdio.h>
#include <stdlib.h>
#include "simlib.h"

#define EVENT_PRODUCE_PIECE        1  /* Event type for proudce a piece. */
#define EVENT_START_TRAVEL     2  /* Event type for start travel . */
#define EVENT_UNLOADING     3  /* Event type for unloading */
#define EVENT_ARRIVE_A      4  /*llegada de montacargas a A*/
#define LIST_MACHINE_A           1  /* List number for machine A. */
#define LIST_MACHINE_B          2  /* List number for machine B. */
#define SAMPST_DELAYS_A        1  /* sampst variable for delays in machine A. */
#define STREAM_PRODUCE_A  1  /* Random-number stream for pieces produce in A. */

int   num_pieces_queue_A, num_N;
float mean_produce_A;
FILE  *infile, *outfile;

void init_model(void);
void arrive_piece(void);
void start_travel(void);
void unloading(void);
void arrive_montacargas_A(void);
void report(void);

int main()
{
    infile  = fopen("Ejercicio_2.7.in",  "r");
    outfile = fopen("Ejercicio_2.7.out", "w");

    /* Read input parameters. */
    init_simlib();

    /* Set maxatr = max(maximum number of attributes per record, 4) */

    maxatr = 4;  /* NEVER SET maxatr TO BE SMALLER THAN 4. */

    /* Initialize the model. */

    init_model();

    /* Run the simulation while more delays are still needed. */

    while (num_custs_delayed < num_delays_required) {

        /* Determine the next event. */

        timing();

        /* Invoke the appropriate event function. */

        switch (next_event_type) {
            case EVENT_ARRIVAL:
                arrive();
                break;
            case EVENT_DEPARTURE:
                depart();
                break;
        }
    }


    fscanf(infile, "%f", &mean_produce_A);
    fprintf(outfile, "prueba %f",mean_produce_A);

    fclose(infile);
    fclose(outfile);

    return 0;
}



float normal(){
    float aux=0, cK=1000;
    for (int k=1;k<=cK;k++){
        aux=aux+(float)rand()/RAND_MAX;
    }
    return desviation*sqrt((float)12/cK)*(aux-(float)cK/2)+mean_intertravel;
}

