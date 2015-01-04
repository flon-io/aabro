
NAME=aabro

default: $(NAME).o

.DEFAULT spec clean:
	$(MAKE) -C tmp/ $@ NAME=$(NAME)

# copy up-to-date version of dep libs into src/
#
stamp:
	cd $(REP) && git log -n 1 | sed 's/^/\/\//' >> ../$(NAME)/$(FIL)
upgrade:
	cp -v ../flutil/src/flutil.* src/
	find src/flutil.* -exec $(MAKE) stamp REP=../flutil FIL={} \;

cs: clean spec

.PHONY: spec clean upgrade cs

