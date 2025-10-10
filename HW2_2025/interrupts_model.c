#include "interrupts_model.h"
#include "xoshiro128.h"

int clock_time = 0;

void simulate(int trial)
{
	int fire;
	int trigger[INTERRUPTS];
	int i, j, active, pending, priority;
	int val;
	
	for (i = 0; i < trial; i++) jump();	//jumps to (#trial) random sequence (seeds RND())
	
	while (clock_time < CYCLES)
	{
		// who is requested
		for (i = 0; i < INTERRUPTS; i++)
			trigger[i] = (interrupt_data[i].prob > RND()) ? 1 : 0;

		// who is active (only one can be active at a time!)
		active = -1;
		for (i = 0; i < INTERRUPTS; i++)
			if (interrupt_data[i].active == 1)
				active = i;

		// who got missed
		for (i = 0; i < INTERRUPTS; i++)
			if (interrupt_data[i].pending == 1 && trigger[i] == 1)
				interrupt_data[i].missed++;

		// who is pending
		for (i = 0; i < INTERRUPTS; i++)
			if (active != -1 && trigger[i] == 1 && (interrupt_data[i].pending != 1))
			{
				interrupt_data[i].pending = 1;
				interrupt_data[i].arrive_time_next = clock_time;
			}

		// highest priority pending
		pending = -1;
		for (i = priority = 0; i < INTERRUPTS; i++)
			if (interrupt_data[i].pending && (priority < interrupt_data[i].priority))
			{
				pending = i;
				priority = interrupt_data[i].priority;
			}

		// who becomes active (only one can be active at a time!)
		if (active != -1) // somebody is active
		{
			if (interrupt_data[active].run_time <= clock_time - interrupt_data[active].start_time)
			{ // somebody has finished executing
				interrupt_data[active].active = 0;
				int val = clock_time - interrupt_data[active].arrive_time_present;
				record_data(active, val);
				if (val > interrupt_data[active].max_latency)
					interrupt_data[active].max_latency = val;
				if (pending != -1)
				{ // Highest priority pending interrupt is made active
					interrupt_data[pending].active = 1;
					interrupt_data[pending].start_time = clock_time;
					interrupt_data[pending].pending = 0;
					interrupt_data[pending].arrive_time_present = interrupt_data[pending].arrive_time_next;
				}
			}
		}
		else
		{
			// nobody is active
			fire = -1; // find highest priority triggered interrupt
			for (i = priority = 0; i < INTERRUPTS; i++)
				if ((trigger[i] == 1) && (priority < interrupt_data[i].priority))
				{ // Found someone with a higher priority
					if (fire != -1)
					{ // Make the lower priority interrupt pending
						interrupt_data[fire].pending = 1;
						interrupt_data[fire].arrive_time_next = clock_time;
					}
					fire = i;
					priority = interrupt_data[i].priority;
				}
			if (fire != -1)
			{ // somebody was triggered
				// fire the interrupt with the highest priority
				interrupt_data[fire].active = 1;
				interrupt_data[fire].start_time = clock_time;
				interrupt_data[fire].arrive_time_present = clock_time;
			}
		}

		clock_time++;
	}
}
