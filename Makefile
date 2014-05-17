CC=gcc
CFLAGS= -Wall

.PHONY: clean

all: markov

markov: markov.c
	$(CC) $(CFLAGS)  $^ -o $@

clean:
	-rm  -f *.o markov
