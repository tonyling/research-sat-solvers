# Edit source walksat.c to set o/s dependent flags


CC = gcc -O3

all:	walksat makewff makequeens

walksat: walksat.c
	$(CC)  walksat.c -lm -o walksat

makewff: makewff.c
	$(CC)  makewff.c -lm -o makewff

makequeens: makequeens.c
	$(CC)  makequeens.c -lm -o makequeens

install: walksat makewff
	cp walksat $(HOME)/bin/
	cp makewff $(HOME)/bin/
	cp makequeens $(HOME)/bin/

clean:
	rm -f walksat makewff makequeens

