CC=gcc
CFLAGS=-Wall

clean:
	rm -rf *.o
	rm -rf *.gch
	rm -rf fcfs
	rm -rf sjf
	rm -rf priority
	rm -rf rr
	rm -rf rr_priority
fcfs: driver.o  FCFS.o Schedule.o cpu.o
	$(CC) $(CFLAGS) -o fcfs driver.o FCFS.o Schedule.o cpu.o
FCFS.o:FCFS.c 
	$(CC) $(CFLAGS) -c FCFS.c 

sjf: driver.o  SJF.o Schedule.o cpu.o
	$(CC) $(CFLAGS) -o sjf driver.o SJF.o Schedule.o cpu.o
SJF.o:SJF.c 
	$(CC) $(CFLAGS) -c SJF.c

priority: driver.o  Priority.o Schedule.o cpu.o
	$(CC) $(CFLAGS) -o priority driver.o Priority.o Schedule.o cpu.o
Priority.o:Priority.c 
	$(CC) $(CFLAGS) -c Priority.c

rr: driver.o  RR.o Schedule.o cpu.o
	$(CC) $(CFLAGS) -o rr driver.o RR.o Schedule.o cpu.o
RR.o:RR.c 
	$(CC) $(CFLAGS) -c RR.c

rr_priority:driver.o  RR_Priority.o Schedule.o cpu.o
	$(CC) $(CFLAGS) -o rr_priority driver.o RR_Priority.o Schedule.o cpu.o
RR_Priority.o:RR_Priority.c 
	$(CC) $(CFLAGS) -c RR_Priority.c

 
driver.o: driver.c 
	$(CC) $(CFLAGS) -c driver.c 

cpu.o: cpu.c cpu.h 
	$(CC) $(CFLAGS) -c cpu.c cpu.h
Schedule.o: Schedule.c Schedule.h
	$(CC) $(CFLAGS) -c Schedule.c Schedule.h
