
CFLAGS=
LDLIBS=
CC=c99
NAME=aabro

# TODO: at some point deploy rodzo and help deploy it
RODZO=../../rodzo/bin/rodzo

SRC=../src
VPATH=$(SRC)


build: $(NAME).o
	cp $(SRC)/$(NAME).h .

spec.c:
	$(RODZO) ../spec

spec: build spec.c
	time ./a.out

vspec: build spec.c
	valgrind --leak-check=full -v ./a.out

