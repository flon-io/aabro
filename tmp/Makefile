
CFLAGS=-I../src -g -Wall -O3
LDLIBS=
CC=c99
VPATH=../src

RODZO=$(shell which rodzo)
ifeq ($(RODZO),)
  RODZO=../../rodzo/bin/rodzo
endif


s.c: ../spec/*_spec.c
	$(RODZO) ../spec -o s.c

s: $(NAME).o $(FLIBS:=.o)

spec: s
	time ./s

vspec: s
	valgrind --leak-check=full -v ./s
	#valgrind --read-var-info=yes --track-origins=yes --leak-check=full -v ./s

dspec: s
	gdb ./s

clean:
	rm -f *.o
	rm -f *.so
	rm -f *.c
	rm -f s

.PHONY: spec vspec dspec clean

