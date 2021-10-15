
CXX = g++
CXXFLAGS = -std=c++11 -Wall -lpthread
OBJECTS = ParallelRadixSort.o ThreadPool.o

ifdef d
CXXFLAGS += -DDEBUG
endif

ifdef m
	CXXFLAGS += -DMIN_SIZE=$(m)
endif

ifdef n
	CXXFLAGS += -DRAND_NUMBER=$(n)
endif

ifdef r
	CXXFLAGS += -DRAND_RANGE=$(r)
endif

ifdef t
	CXXFLAGS += -DTHREADS=$(t)
endif

ifeq ($(show),true)
	CXXFLAGS += -DPRINT_TEST
endif


all: $(OBJECTS)


$(OBJECTS): ThreadPool.h ParallelRadixSort.h ParallelRadixSort.cpp ThreadPool.cpp


test: $(OBJECTS) test.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS)


clean:
	rm -f test $(OBJECTS)


.PHONY: clean
