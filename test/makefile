

SRC=$(wildcard ../*.c)

all:$(patsubst %.c,test-%,$(wildcard *.c))

test-%:%.c.o $(patsubst %.c,%.c.o,$(SRC))
	gcc -g -o $@ $^

%.c.o:%.c
	gcc -g -c -o $@ $^

clean:
	rm -rf $(patsubst %.c,test-%,$(wildcard *.c)) 
	rm -rf $(patsubst %.c,test-%.c.o,$(wildcard *.c)) 
	rm -rf $(patsubst %.c,%.c.o,$(SRC))
