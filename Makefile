CHAN_INC=-I../pthreadChannel
CHAN_OBJ=../pthreadChannel/chan.o

SQLITE_INC=
SQLITE_LIB=-lsqlite3

CFLAGS = -I. $(CHAN_INC) $(SQLITE_INC) -Os -g
OBJS=main.o agent.o tableDrivenAgent.o reflexVacuumAgent.o simpleReflexAgent.o modelBasedReflexAgent.o simpleProblemSolvingAgent.o

all: main

clean:
	rm -f main $(OBJS)\

main: $(OBJS)
	$(CC) -o $@ $(OBJS) $(CHAN_OBJ) $(SQLITE_LIB) -lpthread

main.o: main.c agent.h tableDrivenAgent.h reflexVacuumAgent.h simpleReflexAgent.h modelBasedReflexAgent.h simpleProblemSolvingAgent.h

agent.o: agent.c agent.h

tableDrivenAgent.o: tableDrivenAgent.c tableDrivenAgent.h agent.h

reflexVacuumAgent.o: reflexVacuumAgent.c reflexVacuumAgent.h agent.h

simpleReflexAgent.o: simpleReflexAgent.c simpleReflexAgent.h agent.h

modelBasedReflexAgent.o: modelBasedReflexAgent.c modelBasedReflexAgent.h agent.h

simpleProblemSolvingAgent.o: simpleProblemSolvingAgent.c simpleProblemSolvingAgent.h agent.h
