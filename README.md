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
      pool.add_task(...)
   }
   ```
   在析构函数结束后，任务就完成了，因此，把`pool`的作用域利用起来



## 并行基数排序
1. 4线程
   ```shell
   make test n=1000000 t=4
   time ./test
   
   real    0m1.430s
   user    0m4.871s
   sys     0m0.111s
   ```

2. 2线程
   ```shell
   make test n=1000000 t=2
   time ./test
   
   real    0m1.705s
   user    0m3.121s
   sys     0m0.020s
   ```

3. 单线程
   ```shell
   make test n=1000000 t=1
   time ./test
   
   real    0m2.680s
   user    0m2.667s
   sys     0m0.000s
   ```shell
