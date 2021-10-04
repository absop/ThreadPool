

CFLAGS =

ifdef d
CFLAGS += -DDEBUG
endif

ifdef m
	CFLAGS += -DMIN_SIZE=$(m)
endif

ifdef n
	CFLAGS += -DRAND_NUMBER=$(n)
endif

ifdef r
	CFLAGS += -DRAND_RANGE=$(r)
endif

ifdef t
	CFLAGS += -DTHREADS=$(t)
endif

ifeq ($(show),true)
	CFLAGS += -DPRINT_TEST
endif


ParallelRadixSort.o: ParallelRadixSort.cpp ThreadPool.h
	g++ -c -o $@ $^ $(CFLAGS) -lpthread


test: ParallelRadixSort.cpp ThreadPool.h test.cpp
	g++ -o $@ test.cpp $(CFLAGS) -lpthread


clean: clean
	rm -f test ParallelRadixSort.o


.PHONY: clean test ParallelRadixSort.o
