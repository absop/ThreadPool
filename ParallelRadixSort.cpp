#include <algorithm>
#include <cstdio>
#include <functional>
#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <random>

#include "ThreadPool.h"

#ifndef MIN_SIZE
    #define MIN_SIZE 45
#endif


class ParallelRadixSort : public ThreadPool {
    using T = unsigned int;

public:
    ParallelRadixSort(size_t threads) : ThreadPool(threads) {}

    // Just add the sorting task
    void sort(std::vector<T> &v)
    {
        size_t size = v.size();
        if (size > 1) {
            add_task([this, &v, size]{
                sort(&v, &v[0], &v[0] + size, 1, false);
            });
        }
    }

private:
    // sort numbers in `v` into the memory pointed to by `begin`
    void sort(std::vector<T> *v, T *begin, T *end, int digit, bool delete_v)
    {
        debug("sort: size=%zu\n", v->size());

        if (!v->size()) {
            return;
        }

        if (end - begin < MIN_SIZE) {
            std::copy(v->begin(), v->end(), begin);
            if (delete_v) {
                delete v;
            }
            std::sort(begin, end, [](T x, T y) {
                return std::to_string(x) < std::to_string(y);
            });
            return;
        }

        std::vector<T> *bucket[10];

        for (int i = -1; i < 10; ++i) {
            bucket[i] = new std::vector<T>;
        }

        for (auto &x : *v) {
            int d = get_digit(x, digit);
            if (d >= 0) {
                bucket[d]->push_back(x);
            } else {
                *begin++ = d;
            }
        }

        if (delete_v) {
            delete v;
        }

        digit += 1;
        for (int i = 0; i < 10; ++i) {
            v = bucket[i];
            if (v->size() > 0) {
                end = begin + v->size();
                add_task([=]{
                    sort(v, begin, end, digit, true);
                });
                begin = end;
            } else {
                delete v;
            }
        }
    }

    // return the the n-th digit if there is, -1 otherwise
    int get_digit(T num, int n)
    {
        int digits[20];
        int count = 0;
        do {
            digits[count++] = num % 10;
            num /= 10;
        }
        while (num > 0);
        count -= n;
        if (count >= 0) return digits[count];
        return -1;
    }
};
