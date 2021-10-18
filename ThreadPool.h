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
  using Task = std::function<void()>;

  public:
    // Deleted operations
    ThreadPool(const ThreadPool &rhs) = delete;
    ThreadPool &operator=(const ThreadPool &rhs) = delete;
    ThreadPool(ThreadPool &&rhs) = delete;
    ThreadPool &operator=(ThreadPool &&rhs) = delete;

    ThreadPool(size_t threads = 1)
        : _threads(threads), _num_active_threads(threads)
    {
        debug("Construct ThreadPool with threads: %zu\n", threads);

        // 活跃线程数加加，
        // 在析构函数之前，`_num_active_threads`无法被减到0，线程不能退出
        // 以此避免线程在析构函数开始执行之前退出
        _num_active_threads += 1;

        for (size_t i = 0; i < threads; ++i)
            _threads[i] = std::thread(&ThreadPool::worker, this);
    }

    ~ThreadPool()
    {
        // 活跃线程数减减，
        // 此后`_num_active_threads`可以被减到0，线程能够退出
        std::unique_lock<std::mutex> _lock(_mtx);
        _num_active_threads -= 1;
        _lock.unlock();

        // 通知工作线程，可以退出
        _cv.notify_all();

        // join all threads
    #ifdef DEBUG
        int i = 0;
    #endif
        for (auto &t : _threads) {
            if (t.joinable()) {
                t.join();
                debug("Destructor: thread %d joined\n", ++i);
            }
        }
    }

    //  copy and submit the task
    void submit(const Task &s)
    {
        std::unique_lock<std::mutex> _lock(_mtx);
        debug("Thread %zu add new task\n", current_thread_id());
        _q.push(s);

        // 在通知之前手动解锁，以避免等待唤醒的线程在被唤醒之后仅仅是再次阻塞
        _lock.unlock();
        _cv.notify_one();
    }


    //  move and submit the task
    void submit(Task &&s)
    {
        std::unique_lock<std::mutex> _lock(_mtx);
        debug("Thread %zu add new task\n", current_thread_id());
        _q.push(std::move(s));

        // 同上
        _lock.unlock();
        _cv.notify_one();
    }


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
    void worker()
    {
        Task task;

        for (; ;) {
            {
                std::unique_lock<std::mutex> _lock(_mtx);

                _num_active_threads -= 1;

                // 等待，直到有任务可执行，或者所有工作线程均以不再活跃
                _cv.wait(_lock, [this] {
                    return (!_q.empty() || _num_active_threads == 0);
                });

                // 无任务可执行，那么进入临界区的条件就是
                // `_num_active_threads == 0`
                // 这时候，由于所有工作线程均处于等待任务状态，
                // 就意味着，不会再有新任务被产生和提交。
                // 于是跳出循环，不再继续无谓的等待。
                if (_q.empty())
                    break;

                _num_active_threads += 1;

                task = std::move(_q.front());
                _q.pop();
            }

            debug("Thread %zu run task, active threads: %d\n",
                  current_thread_id(), _num_active_threads);

            task();
        }
        // 通知所有线程退出，也许可以考虑串联通知
        _cv.notify_all();

        debug("Thread %zu exited, active threads: %d\n",
              current_thread_id(), _num_active_threads);
    }
};
