#include <stdio.h>
#include <stdlib.h>
#include "simlib.h"

#define EVENT_PRODUCE_PIECE        1  /* Event type for proudce a piece. */
#define EVENT_START_TRAVEL     2  /* Event type for start travel . */
#define EVENT_UNLOADING     3  /* Event type for unloading */
#define LIST_MACHINE_A           1  /* List number for machine A. */
#define LIST_MONTACARGAS          2  /* List number for montacargas. */
#define LIST_MACHINE_B          3  /* List number for machine B. */
#define SAMPST_DELAYS_A        1  /* sampst variable for delays in machine A. */
#define STREAM_INTERARRIVAL  1  /* Random-number stream for interarrivals. */
#define STREAM_SERVICE       2  /* Random-number stream for service times. */

int main()
{
    printf("Hello world!\n");
    return 0;
}
