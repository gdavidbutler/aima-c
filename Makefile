CHAN_INC=-I../pthreadChannel
CHAN_OBJS=../pthreadChannel/chan.o ../pthreadChannel/chanFifo.o

CFLAGS = -I. $(CHAN_INC) -Os -g
OBJS=main.o tableDrivenAgent.o reflexVacuumAgent.o

all: main

clean:
	rm -f main $(OBJS)\

main: tableDrivenAgent.h reflexVacuumAgent.h $(OBJS)
	$(CC) -o $@ $(OBJS) $(CHAN_OBJS) -lpthread

main.o: main.c tableDrivenAgent.h

tableDrivenAgent.o: tableDrivenAgent.c tableDrivenAgent.h

reflexVacuumAgent.o: reflexVacuumAgent.c reflexVacuumAgent.h
