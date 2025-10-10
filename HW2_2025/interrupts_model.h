#ifndef _INTERRUPTS_MODEL_H
#define _INTERRUPTS_MODEL_H

#include <stdio.h>
#include <stdlib.h>

#define CYCLES 1000000
#define INTERRUPTS 3

typedef struct INTERRUPT_TAG
{
    double prob;             /* Probability of interrupt occurence */
    int priority;            /* Interrupt Priority - higher number => higher priority */
    int run_time;            /* Time to service the interrupt */
    int start_time;          /* Start time for the interrupt routine */
    int arrive_time_present; /* Arrive time for the interrupt currently being serviced */
    int arrive_time_next;    /* Arrive time for the interrupt in queue */
    int active;              /* active = 1 => Current interrupt is being serviced */
    int missed;              /* Stores the count of missed interrupts */
    int pending;             /* pending = 1 => Current interrupt is in queue, some other interrupt is active */
    int max_latency;
} INTERRUPT;

// exact parameters defined in "main.c"
extern struct INTERRUPT_TAG interrupt_data[INTERRUPTS];

// run this function only once, or you have to reset interrupt_data manually.
void simulate(int trial);

// this function will be called by simulate() a lot to dump data
extern void record_data(int interrupt_number, int service_time);

#endif
