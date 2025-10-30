# Forrest Brewer 10/27/2009

CC  = gcc
RM = rm

# CFLAGS  = -g -c -Wall -o$@
CFLAGS  = -O -c -Wall  -o$@

elevator: main.o elevator.o qepn.o xoshiro128starstar.o
	$(CC) -oelevator main.o elevator.o qepn.o xoshiro128starstar.o

elevator.o: elevator.c
	$(CC) $(CFLAGS) $<

main.o: main.c
	$(CC) $(CFLAGS) $<

qepn.o: qepn.c
	$(CC) $(CFLAGS) $<

xoshiro128starstar.o: xoshiro128starstar.c
	$(CC) $(CFLAGS) $<

clean:
	-$(RM) *.o elevator



