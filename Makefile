
CXX = g++
CXXFLAGS = -std=c++17 -Wall -lpthread
HEADERS = Options.h ParallelRadixSort.h ThreadPool.h

ifdef d
	CXXFLAGS += -DDEBUG
endif

ifdef m
	CXXFLAGS += -DRADIX_SORT_MIN_SIZE=$(m)
endif

ifdef top
	CXXFLAGS += -DTEST_OPTION_PARSER
endif

ifeq ($(OS),Windows_NT)
TEST_EXECUTABLE = test.exe
test: $(TEST_EXECUTABLE)
.PHONY: clean help test
else
TEST_EXECUTABLE = test
.PHONY: clean help
endif


$(TEST_EXECUTABLE): test.cpp $(HEADERS)
	$(CXX) -o $@ test.cpp $(CXXFLAGS)


clean:
	rm -f $(TEST_EXECUTABLE)


help:
	@echo "make $(TEST_EXECUTABLE) [variables...]"
	@echo "variables:"
	@echo "  d=ANY      debug or not"
	@echo "  m=INT      vector size threshold for radix sort"
