
NAME=aabro
FLIBS=flutil

default: $(NAME).o

.DEFAULT spec clean:
	$(MAKE) -C tmp/ $@ NAME=$(NAME) FLIBS="$(FLIBS)"

.PHONY: spec clean

