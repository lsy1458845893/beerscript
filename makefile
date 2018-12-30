
SRC=$(wildcard ./*.c)


./test:$(patsubst %.c,%.c.o,$(SRC))
	gcc -o $@ $^

%.c.o:%.c
	gcc -c -o $@ $^

clean:
	rm -rf $(patsubst %.c,%.c.o,$(SRC)) ./test

