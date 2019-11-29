#include "ThreadPool.h"

struct A {
};

void foo(const A &) {}

int main() {
    ThreadPool pool(8);

    auto task0 = pool.exec(foo, A());
    task0.get();
    int a = 1;
    auto task1 = pool.exec([a]() { return a; });
    std::cout << task1.get() << std::endl;

    auto task2 = pool.exec([a]() { return a + 1; });
    std::cout << task2.get() << std::endl;

    auto task3 = pool.exec([a]() { return a + 2; });
    std::cout << task3.get() << std::endl;

    std::cout << "OK" << std::endl;
    return 0;
}
