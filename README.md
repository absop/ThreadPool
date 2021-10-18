# ThreadPool
ThreadPool 是一个简单的C++**线程池**，用于支持动态并行任务。
仓库链接： [ThreadPool](https://github.com/absop/ThreadPool)


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


# 并行基数排序

## 编译

### 基本编译命令
   ```shell
   absop@walle:ThreadPool$ make clean
   rm -f test
   absop@walle:ThreadPool$ make
   g++ -o test test.cpp -std=c++17 -Wall -lpthread
   absop@walle:ThreadPool$ time ./test

   real    0m0.017s
   user    0m0.000s
   sys     0m0.007s
   ```

### 调试[线程池](ThreadPool.h)和[多线程基数排序](ParallelRadixSort.h)
   ```shell
   absop@walle:ThreadPool$ make clean
   rm -f test
   absop@walle:ThreadPool$ make d=t
   g++ -o test test.cpp -std=c++17 -Wall -lpthread -DDEBUG
   absop@walle:ThreadPool$
   ```

### 调试[命令行参数解析器](Options.h)
   ```shell
   absop@walle:ThreadPool$ make clean
   rm -f test
   absop@walle:ThreadPool$ make top=t
   g++ -o test test.cpp -std=c++17 -Wall -lpthread -DTEST_OPTION_PARSER
   absop@walle:ThreadPool$
   ```

### 设置使用`std::sort`的阈值
   ```shell
   absop@walle:ThreadPool$ make clean
   rm -f test
   absop@walle:ThreadPool$ make m=45
   g++ -o test test.cpp -std=c++17 -Wall -lpthread -DRADIX_SORT_MIN_SIZE=45
   absop@walle:ThreadPool$
   ```


## 帮助信息

```shell
absop@walle:ThreadPool$ ./test -?
Usage: test [options...]
Test program for the header `ParallelRadixSort.h`

Options:
  -c, --column <column>      columns per line when printing
  -n, --number <number>      the number of random numbers
  -r, --range <lb> <ub>      the range of random numbers
  -s, --show                 print sorting result or not
  -t, --threads <threads>    number of threads to be used
  -?, --help                 print this message and exit

Examples:
  ./test -t=4 --range 100 200
  ./test -t 2 --range 1 10000 -n=100000000
```


## 默认参数
- 4线程
- 随机数范围 [1, 100000000]
- 100个随机数
- 不打印排序结果
- 打印时，每行7列


## 使用示例

以每行5列输出排序结果
   ```shell
   absop@walle:ThreadPool$ ./test -s -c=5
   12820005   13451203   1622649    17027214   18235992
   22391013   23846771   24315082   25262475   26838739
   27216237   27540688   27887629   28112284   28334697
   29370661   31324816   32624668   33565602   3535383
   35895215   36738842   37359944   39208517   39217355
   39282809   42139844   42478722   42540563   42692586
   44627922   46902079   47494754   47849757   4810836
   4853775    48853389   49751816   50514613   51202872
   5121645    51593080   52487947   52796598   53116104
   53884331   54167739   54172565   54481378   5467047
   54945310   60229415   60447941   6193377    64377819
   64591678   64694333   66625024   66870490   6835363
   68632492   69421471   69465616   70230151   70402215
   71704540   73900067   74181448   7432941    75272563
   75287148   7541664    77039016   77269052   77351389
   77873306   77943230   78382620   787226     78761985
   801        84653530   84975452   86053826   86511027
   86529980   88787930   90471148   90866396   91795343
   92510844   9278448    92976645   93090434   93448001
   9404434    95147484   95582749   96225670   96919440
   ```


## 性能测试

用于测试的机器的配置信息

- Intel(R) Core(TM) i5-6200U CPU @ 2.30GHz   2.40 GHz
- 8.00 GB (7.85 GB 可用)
- WSL 2

测试结果

1. 4线程
   ```shell
   absop@walle:ThreadPool$ time ./test -n 1000000 -t=4
   
   real    0m1.422s
   user    0m4.749s
   sys     0m0.132s
   ```

2. 2线程
   ```shell
   absop@walle:ThreadPool$ time ./test -n 1000000 -t=2
   
   real    0m1.788s
   user    0m3.285s
   sys     0m0.030s
   ```

3. 单线程
   ```shell
   absop@walle:ThreadPool$ time ./test -n 1000000 -t=1
   
   real    0m2.633s
   user    0m2.582s
   sys     0m0.040s
   ```
