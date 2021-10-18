#include <cstdio>
#include <functional>
#include <map>
#include <random>

#include "Options.h"
#include "ParallelRadixSort.h"


static Options options = {
    "Usage: test [options...]",
    "Test program for the header `ParallelRadixSort.h`",
    "",
    "Options:",
    "  -(c|-column) <column>      columns per line when printing",
    "  -(n|-number) <number>      the number of random numbers",
    "  -(r|-range) <lb> <ub>      the range of random numbers",
    "  -(s|-show)                 print sorting result or not",
    "  -(t|-threads) <threads>    number of threads to be used",
    "  -(?|-help)                 print this message and exit",
    "",
    "Examples:",
    "  ./test -t=4 --range 100 200",
    "  ./test -t 2 --range 1 10000 -n=100000000",
};


int main(int argc, char *argv[])
{
    options.parse(argc, argv);

    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned int> distribution(
        options.get("r", 0, 1), options.get("r", 1, 100000000));
    auto dice = std::bind(distribution, generator);

    std::vector<unsigned int> v(options.get("n", 100));
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] = dice();
    }

    ParallelRadixSort(options.get("-threads", 4)).sort(v);

    if (options.has("-show")) {
        int count = 0;
        for (size_t i = 0; i < v.size(); ++i) {
            if (++count == options.get("-column", 7)) {
                printf("%-10u\n", v[i]);
                count = 0;
            } else {
                printf("%-10u ", v[i]);
            }
        }
        if (count != 0) {
            printf("\n");
        }
    }
}
