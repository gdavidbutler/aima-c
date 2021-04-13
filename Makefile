CHAN_INC=-I../pthreadChannel
CHAN_OBJS=../pthreadChannel/chan.o ../pthreadChannel/chanFifo.o

CFLAGS = -I. $(CHAN_INC) -Os -g

all: \
  tableDrivenAgentRun \
  reflexVacuumAgentRun \
  simpleReflexAgentRun \
  modelBasedReflexAgentRun \

clean:
	rm -f tableDrivenAgentRun
	rm -f reflexVacuumAgentRun
	rm -f simpleReflexAgentRun
	rm -f modelBasedReflexAgentRun

tableDrivenAgent.c: tableDrivenAgent.h
tableDrivenAgentRun.c: tableDrivenAgent.h
tableDrivenAgentRun: tableDrivenAgentRun.c tableDrivenAgent.c
	$(CC) $(CFLAGS) -o $@ $< tableDrivenAgent.c $(CHAN_OBJS) -lpthread

reflexVacuumAgent.c: reflexVacuumAgent.h
reflexVacuumAgentRun.c: reflexVacuumAgent.h
reflexVacuumAgentRun: reflexVacuumAgentRun.c reflexVacuumAgent.c
	$(CC) $(CFLAGS) -o $@ $< reflexVacuumAgent.c $(CHAN_OBJS) -lpthread

simpleReflexAgent.c: simpleReflexAgent.h
simpleReflexAgentRun.c: simpleReflexAgent.h
simpleReflexAgentRun: simpleReflexAgentRun.c simpleReflexAgent.c
	$(CC) $(CFLAGS) -o $@ $< simpleReflexAgent.c $(CHAN_OBJS) -lpthread

modelBasedReflexAgent.c: modelBasedReflexAgent.h
modelBasedReflexAgentRun.c: modelBasedReflexAgent.h
modelBasedReflexAgentRun: modelBasedReflexAgentRun.c modelBasedReflexAgent.c
	$(CC) $(CFLAGS) -o $@ $< modelBasedReflexAgent.c $(CHAN_OBJS) -lpthread
