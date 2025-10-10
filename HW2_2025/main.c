#include "interrupts_model.h"
#include <stdlib.h>

// edit the interrupt data here
struct INTERRUPT_TAG interrupt_data[INTERRUPTS] = {
	{.prob = 0.1, .priority = 1, .run_time = 3, .max_latency = 3, .active = 0, .missed = 0, .pending = 0, .start_time = 0, .arrive_time_present = 0, .arrive_time_next = 0},
	{.prob = 0.03, .priority = 2, .run_time = 5, .max_latency = 5, .active = 0, .missed = 0, .pending = 0, .start_time = 0, .arrive_time_present = 0, .arrive_time_next = 0},
    {.prob = 0.001, .priority = 3, .run_time = 7, .max_latency = 5, .active = 0, .missed = 0, .pending = 0, .start_time = 0, .arrive_time_present = 0, .arrive_time_next = 0}};

#define MAX_TIME 100
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

int histogram[INTERRUPTS][MAX_TIME];

int main(int argc, char *argv[])
{
    printf("program begun \n");
    int i, j, foo;
    printf("input trial? ");
    scanf("%d",&foo);
    
    for (i = 0; i < INTERRUPTS; i++)
    {
        for (j = 0; j < MAX_TIME; j++)
        {
            histogram[i][j] = 0;
        }
    }
    simulate(foo);// this is the seed
    //File stuff
    FILE *fptr;
    fptr = fopen("histhw2partc.txt", "w");
    if (fptr == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    printf("Number of test cycles %d\n", CYCLES);
    for (i = 0; i < INTERRUPTS; i++)
    {
        printf("Number of missed interrupt %d's: %d\n", i, interrupt_data[i].missed);
        printf("Max latency for interrupt %d: %d\n", i, interrupt_data[i].max_latency);
        fprintf(fptr, "Histogram data for interrupt %d\n", i);
        //printf("Histogram data for interrupt %d\n", i);
        for (j = 0; j < MIN(interrupt_data[i].max_latency + 1, MAX_TIME); j++)
            fprintf(fptr, "%3d,%d\n", j, histogram[i][j]);
            //printf("Service Cycles: %3d - Count: %d\n", j, histogram[i][j]);
            
        printf("\n");
    }
    return 0;
}

void record_data(int interrupt_number, int service_time)
{
    if (service_time >= MAX_TIME)
    {
        fprintf(stderr, "record_data(%d): service time larger than histogram size!\n", service_time);
        exit(1);
    }
    histogram[interrupt_number][service_time]++;
}

