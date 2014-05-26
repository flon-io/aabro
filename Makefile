
NAME=aabro
FLIBS=flutil

default: $(NAME).o

.DEFAULT spec clean:
	$(MAKE) -C tmp/ $@ NAME=$(NAME) FLIBS="$(FLIBS)"

# copy updated version of dep libs into src/
#
upgrade:
	cp -v ../flutil/src/flutil.* src/

.PHONY: spec clean upgrade

