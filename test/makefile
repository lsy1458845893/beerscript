

SRC=$(wildcard ../src/*.c)

CFLAGS+=-I../src
CFLAGS+=-g


all:$(patsubst %.c,test-%,$(wildcard *.c))

test-%:%.c.o $(patsubst %.c,%.c.o,$(SRC))
	gcc $(CFLAGS) -o $@ $^

%.c.o:%.c
	gcc $(CFLAGS) -c -o $@ $^

clean:
	rm -rf $(patsubst %.c,test-%,$(wildcard *.c)) 
	rm -rf $(patsubst %.c,test-%.c.o,$(wildcard *.c)) 
	rm -rf $(patsubst %.c,%.c.o,$(SRC))
