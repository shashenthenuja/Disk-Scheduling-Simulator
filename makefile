CC = gcc
LINK = -pthread
CFLAGS = -Wall -ansi -pedantic -g
OBJA = scheduler.o fileIO.o
OBJB = simulator.o fileIO.o
EXECA = scheduler
EXECB = simulator
ALL = $(EXECA) $(EXECB)

all : $(ALL)

scheduler : $(EXECA)

simulator : $(EXECB)

$(EXECA) : $(OBJA)
	$(CC) $(OBJA) -o $(EXECA)

$(EXECB) : $(OBJB)
	$(CC) $(LINK) $(OBJB) -o $(EXECB)

scheduler.o : scheduler.c fileIO.h scheduler.h
	$(CC) $(CFLAGS) scheduler.c -c

simulator.o : simulator.c fileIO.h simulator.h
	$(CC) $(LINK) $(CFLAGS) simulator.c -c

fileIO.c : fileIO.c fileIO.h
	$(CC) $(CFLAGS) fileIO.c -c

clean :
	rm -f $(EXECA) $(EXECB) $(OBJA) $(OBJB)