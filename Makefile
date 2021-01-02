CHAN_INC=-I../pthreadChannel
CHAN_OBJ=../pthreadChannel/chan.o

#SQLITE_INC=
#SQLITE_LIB=-lsqlite3

CFLAGS = -I. $(CHAN_INC) $(SQLITE_INC) -Os -g
OBJS=main.o agent.o reflexVacuumAgent.o

all: main

clean:
	rm -f main $(OBJS)\

main: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CHAN_OBJ) $(SQLITE_LIB) -lpthread

main.o: main.c agent.h reflexVacuumAgent.h

agent.o: agent.c agent.h

reflexVacuumAgent.o: reflexVacuumAgent.c reflexVacuumAgent.h agent.h
