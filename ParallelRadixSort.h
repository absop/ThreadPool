#include <vector>

#include "ThreadPool.h"

#ifndef MIN_SIZE
    #define MIN_SIZE 45
#endif


class ParallelRadixSort : public ThreadPool {
    using T = unsigned int;

public:
    ParallelRadixSort(size_t threads) : ThreadPool(threads) {}

    // Just add the sorting task
    void sort(std::vector<T> &v);

private:
    // sort numbers in `v` into the memory pointed to by `begin`
    void sort(std::vector<T> *v, T *begin, T *end, int digit, bool delete_v);

    // return the the n-th digit if there is, -1 otherwise
    int get_digit(T num, int n);
};
