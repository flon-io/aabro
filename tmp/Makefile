
CFLAGS=
LDLIBS=
CC=c99
NAME=aabro

# TODO: at some point deploy rodzo and help deploy it
RODZO=../../rodzo/bin/rodzo

VPATH=../src


build: $(NAME).o
	cp ../src/$(NAME).h .

spec.c: ../spec/*_spec.c
	$(RODZO) ../spec

spec: build spec.c
	time ./a.out

vspec: build spec.c
	valgrind --leak-check=full -v ./a.out

