#include <iostream>
#include <functional>
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>

class ThreadPool {
    using Task = std::function<void()>;
    std::vector<std::thread> threds;
    std::queue<Task> taskQueue;
    std::mutex m;
    std::condition_variable empty_queue;
    std::atomic<bool> flag;
public:
    explicit ThreadPool(size_t poolSize) : flag(true) {
        for (size_t i = 0; i < poolSize; i++) {
            threds.emplace_back(
                    [this]() {
                        while (flag == true) {
                            std::unique_lock<std::mutex> lock(m);
                            if (taskQueue.empty()) {
                                empty_queue.wait(lock);
                            } else {
                                auto task = taskQueue.front();
                                taskQueue.pop();
                                lock.unlock();
                                task();
                            }

                        }
                    }
            );
        }
    }

    ~ThreadPool() {
        flag = false;
        empty_queue.notify_all();
        for (size_t i = 0; i < threds.size(); i++) {
            threds[i].join();
        }
    }

    template<class Func, class... Args>
    auto exec(Func func, Args... args) -> std::future<decltype(func(args...))> {
        using task_type = decltype(func(args...));
        auto one_task = std::make_shared<std::packaged_task<task_type()>>([func, args...]() {
                                                                              return func(args...);
                                                                          }
        );
        std::unique_lock<std::mutex> lock(m);
        taskQueue.push([one_task]() {
            (*one_task)();
        });
        empty_queue.notify_one();
        return one_task->get_future();
    }
};
