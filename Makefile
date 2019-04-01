CFLAGS = -Wall -Wextra -I. -Os -g

all: main

clean:
	rm -f environment.o
	rm -f agent.o

clobber: clean
	rm -f main

environment.o: environment.c environment.h
	cc $(CFLAGS) -c environment.c

agent.o: agent.c agent.h environment.h
	cc $(CFLAGS) -c agent.c

main: main.c environment.h agent.h environment.o agent.o
	cc $(CFLAGS) -o main main.c environment.o agent.o
