
default: build

.DEFAULT:
	cd tmp/ && $(MAKE) $@

spec:
	cd tmp/ && $(MAKE) spec

.PHONY: clean spec

clean:
	rm -f tmp/*.o
	rm -f tmp/*.so
	rm -f tmp/*.c
	rm -f tmp/*.h

