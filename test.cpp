#include "radix_sort.cpp"


#ifndef RAND_RANGE
    #define RAND_RANGE 100000000
#endif

#ifndef RAND_NUMBER
    #define RAND_NUMBER 10000
#endif

#ifndef COLUMN
    #define COLUMN 7
#endif

#ifndef THREADS
    #define THREADS 4
#endif


int main(int argc, char *argv[])
{
    std::default_random_engine generator;
    std::uniform_int_distribution<unsigned int> distribution(1, RAND_RANGE);
    auto dice = std::bind(distribution, generator);

    std::vector<unsigned int> v(RAND_NUMBER);
    for (size_t i = 0; i < v.size(); ++i) {
        v[i] = dice();
    }

    ParallelRadixSort(THREADS).sort(v);

#ifdef PRINT_TEST
    size_t count = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (++count == COLUMN) {
            printf("%-10u\n", v[i]);
            count = 0;
        } else {
            printf("%-10u ", v[i]);
        }
    }
#endif
}
