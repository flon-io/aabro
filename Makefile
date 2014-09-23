
NAME=aabro

default: $(NAME).o

.DEFAULT spec clean:
	$(MAKE) -C tmp/ $@ NAME=$(NAME)

# copy up-to-date version of dep libs into src/
#
upgrade:
	cp -v ../flutil/src/flutil.* src/

cs: clean spec

.PHONY: spec clean upgrade cs

