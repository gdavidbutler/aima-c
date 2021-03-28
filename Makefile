CHAN_INC=-I../pthreadChannel
CHAN_OBJ=../pthreadChannel/chan.o

CFLAGS = -I. $(CHAN_INC) -Os -g
OBJS=main.o tableDrivenAgent.o reflexVacuumAgent.o

all: main

clean:
	rm -f main $(OBJS)\

main: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CHAN_OBJ) -lpthread

main.o: main.c tableDrivenAgent.h

tableDrivenAgent.o: tableDrivenAgent.c tableDrivenAgent.h

reflexVacuumAgent.o: reflexVacuumAgent.c reflexVacuumAgent.h
