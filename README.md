# ThreadPool
C++ Thread Pool (C++线程池)

C++线程池的简单实现


## 线程池的使用方法
1. 继承，参考[ParallelRadixSort.cpp](ParallelRadixSort.cpp)
   ```cpp
   #include "ThreadPool.h"

   class ParallelRadixSort : public ThreadPool {
   public:
      ...

   private:
      ...
   }

   ```

2. 就像使用一般数据结构那样
   ```cpp
   {
      pool = ThreadPool(4)
      pool.submit(...)
   }
   ```
   在析构函数结束后，任务就完成了，因此，把`pool`的作用域利用起来



## 并行基数排序测试
1. 默认
   ```shell
   absop@walle:ThreadPool$ make clean
   rm -f test ParallelRadixSort.o ThreadPool.o
   absop@walle:ThreadPool$
   absop@walle:ThreadPool$ make test
   g++ -std=c++11 -Wall -lpthread   -c -o ParallelRadixSort.o ParallelRadixSort.cpp
   g++ -std=c++11 -Wall -lpthread   -c -o ThreadPool.o ThreadPool.cpp
   g++ -o test ParallelRadixSort.o ThreadPool.o test.cpp -std=c++11 -Wall -lpthread
   absop@walle:ThreadPool$ time ./test

   real    0m0.036s
   user    0m0.062s
   sys     0m0.000s
   ```

2. 4线程
   ```shell
   absop@walle:ThreadPool$ make test n=1000000 t=4
   g++ -o test ParallelRadixSort.o ThreadPool.o test.cpp -std=c++11 -Wall -lpthread -DRAND_NUMBER=1000000 -DTHREADS=4
   absop@walle:ThreadPool$ time ./test

   real    0m1.494s
   user    0m5.175s
   sys     0m0.040s
   ```

3. 2线程
   ```shell
   absop@walle:ThreadPool$ make test n=1000000 t=2
      time ./testg++ -o test ParallelRadixSort.o ThreadPool.o test.cpp -std=c++11 -Wall -lpthread -DRAND_NUMBER=1000000 -DTHREADS=2
   absop@walle:ThreadPool$ time ./test

   real    0m1.959s
   user    0m3.571s
   sys     0m0.040s
   ```

4. 单线程
   ```shell
   absop@walle:ThreadPool$ make test n=1000000 t=1
      time ./testg++ -o test ParallelRadixSort.o ThreadPool.o test.cpp -std=c++11 -Wall -lpthread -DRAND_NUMBER=1000000 -DTHREADS=1
   absop@walle:ThreadPool$ time ./test

   real    0m2.773s
   user    0m2.732s
   sys     0m0.023s
   ```
