
SRC=$(wildcard ./src/*.c)

CFLAGS=-I./src
CFLAGS=-g

./main:$(patsubst %.c,%.c.o,$(SRC)) ./main.c.o
	gcc $(CFLAGS) -o $@ $^

%.c.o:%.c
	gcc $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(patsubst %.c,%.c.o,$(SRC)) ./main.c.o ./main

