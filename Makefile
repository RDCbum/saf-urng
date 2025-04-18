CC=gcc
CFLAGS=-O3 -std=c11 -Wall
SRC=src/saf_rng.c
HDR=src/saf_rng.h

bench: $(SRC) examples/bench.c $(HDR)
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f bench

gen: $(SRC) examples/gen.c $(HDR)
	$(CC) $(CFLAGS) $^ -o $@

practrand: gen
	./gen 134217728 | RNG_test.exe stdin64 -multithreaded > practrand.log
	@echo "PractRand finished. Summary:"
	@tail -n 5 practrand.log
