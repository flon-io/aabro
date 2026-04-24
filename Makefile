
CFLAGS = -Isrc

SRCS != ls src/*.c
OBJS := $(SRCS:.c=.o)

RODZO = ../rodzo/bin/rodzo


$(OBJS):
	$(CC) $(CFLAGS) -c $< -o $@

tmp/spec.c: spec/*_spec.c
	$(RODZO) spec -o tmp/spec.c

tmp/spec: tmp/spec.c $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) tmp/spec.c -o tmp/spec

spec: tmp/spec
	time tmp/spec

clean:
	rm -f src/*.o
	rm -f tmp/spec.c

.PHONY: clean

