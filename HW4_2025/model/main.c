/*  Elevator simulation test code */
/*  usage: 'elevator' [output trace file] */
/*  runs the qpnano elevator model TOTAL_SIM_TIME seconds */
/*  with random floor calls every CALLTIME seconds */
/*  the trace file records every dispatch/even transision of the model */
/*  default setup tracks the average number of ticks (seconds) between floor call and arrival */

#include "qpn_port.h"
#include "elevator.h"
#include "my_rand.h"
#include <stdio.h>
#include <stdlib.h>


/* local objects -----------------------------------------------------------*/
static FILE *l_outFile = (FILE *)0;
static void dispatch(QSignal sig);
static int outf;
int seed;

#define CALLTIME 20 //200 or 100 or 50 or 20 or 10
#define TOTAL_SIM_TIME 100000000
#define TRUE 1
#define FALSE 0

/*..........................................................................*/
int main(int argc, char *argv[]) {
	QHsmTst_ctor();                                  /* instantiate the HSM */

	if (argc > 1) {                                  /* file name provided? */
		outf = TRUE;				 /* write output trace */
		l_outFile = fopen(argv[1], "w");
	} else  				 /* just do the stats */
		outf = FALSE;

    printf("Input random seed\n");
    if (!scanf("%d",&seed)) exit(1);
    my_srand(seed);

	printf("Elevator Controller Model, built on %s at %s, QP-nano %s\n"
		"output saved to %s\n",
		__DATE__, __TIME__, QP_getVersion(),
		argv[1]);

	if (outf) fprintf(l_outFile, "QHsmTst example, QP-nano %s\n",
			  QP_getVersion());

	QHsm_init((QHsm *)&HSM_QHsmTst);    /* take the initial transitioin */
	
	printf("Testing elevator controller model!\n\n");

	int floor_call = 0;
	int switcher = 0;
	int emergency_inst = getcompletedEm(); //count instance of emergency
	int restart_time = 500;
	int local_time = 0;
	int serv_time = 0;
	int serv_start = 0;
	simTime = 0;

	FILE *fptr;
	fptr = fopen("hw4realstats.txt", "w");
	

	printf("Entering random call mode!\n\n");
			
	while (simTime < TOTAL_SIM_TIME){
		if (local_time == restart_time && switcher == 0){ /*after 500s*/
			switcher = 1; /*Switch to emergency mode*/
			serv_start = simTime;
			printf("EMERGENCY MODE ACTIVATED\n");
			dispatch(EMERGENCY_KEY);
		}
		if (getcompletedEm() == emergency_inst+1){
			serv_time = simTime - serv_start;
			emergency_inst+=1; /*update the prev count*/
			fprintf(fptr, "Servtime %d\n", serv_time);
			switcher = 0; /*switch to normal mode*/
			local_time = 0; /*reset time*/
			printf("ELEVATOR NORMAL MODE\n");
		}
		if (simTime%CALLTIME == 0){
			if (switcher == 0){ /*only rnd event wehn we in normal mode*/
				floor_call = (abs(my_rand()) % 5) + 1;
				switch (floor_call) {
					case 1: {
						dispatch(F1_SIG);
						BSP_display("F1 called\n");
						break;
						}
					case 2: {
						dispatch(F2_SIG);
						BSP_display("F2 called\n");
						break;
						}
					case 3: {
						dispatch(F3_SIG);
						BSP_display("F3 called\n");
						break;
						}
					case 4: {
						dispatch(F4_SIG);
						BSP_display("F4 called\n");
						break;
						}
					case 5: {
						dispatch(F5_SIG);
						BSP_display("F5 called\n");
						break;
						}
					}
				}
			}
			//debugPrintState();
			dispatch(TICK_SIG);
			if (switcher == 0){
				local_time++;
			}
			simTime++;
	}
	fclose(fptr);
	printf("Results for call time:%d seconds, simulation time:%d seconds\n", CALLTIME, TOTAL_SIM_TIME);
	printf("emergency_inst =  %d \n", emergency_inst);
 	dispatch(PRINT_SIG); 
	if (outf) fclose(l_outFile);

}

/*..........................................................................*/
void Q_onAssert(char const Q_ROM * const Q_ROM_VAR file, int line) {
    fprintf(stderr, "Assertion failed in %s, line %d", file, line);
    exit(-1);
}
/*..........................................................................*/
void BSP_display(char const *msg) {
    if (outf) fprintf(l_outFile,"%s", msg);
}
/*..........................................................................*/
void BSP_exit(void) {
    printf("Bye, Bye!");
    exit(0);
}
/*..........................................................................*/
static void dispatch(QSignal sig) {
    if (outf) fprintf(l_outFile, "\n%c:", 'A' + sig - F1_SIG);
    Q_SIG((QHsm *)&HSM_QHsmTst) = sig;
    QHsm_dispatch((QHsm *)&HSM_QHsmTst);              /* dispatch the event */
}
