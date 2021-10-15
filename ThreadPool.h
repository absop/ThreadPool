#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

// https://embeddedartistry.com/blog/2017/02/08/implementing-an-asynchronous-dispatch-queue/

#ifdef DEBUG
  #include <cstdio>
  #define debug printf
#else
  #define debug(...)
#endif


class ThreadPool {
    typedef std::function<void()> Task;

  public:
    ThreadPool(size_t threads = 1);
    ~ThreadPool();

    //  copy and submit the task
    void submit(const Task &t);

    //  move and submit the task
    void submit(Task &&t);

    // Deleted operations
    ThreadPool(const ThreadPool &rhs) = delete;
    ThreadPool &operator=(const ThreadPool &rhs) = delete;
    ThreadPool(ThreadPool &&rhs) = delete;
    ThreadPool &operator=(ThreadPool &&rhs) = delete;

#ifdef DEBUG
    size_t current_thread_id()
    {
        std::thread::id id = std::this_thread::get_id();
        for (size_t i = 0; i < _threads.size(); ++i) {
            if (_threads[i].get_id() == id)
                return i + 1;
        }
        return 0;
    }
#endif

  private:
    std::condition_variable _cv;
    std::mutex _mtx;
    std::queue<Task> _q;
    std::vector<std::thread> _threads;
    int _num_active_threads;

    // 工作函数，运行于`工作线程`之中
    void worker();
};
