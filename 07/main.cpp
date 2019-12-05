#include "vector.h"

int main() {
    Vector<int> v;
    assert(v.empty());
    assert(v.size() == 0);

    v.push_back(1);
    assert(!v.empty());
    assert(v[0] == 1);

    v.pop_back();
    assert(v.empty());

    v.push_back(3);
    v.push_back(2);
    v.push_back(1);
    assert(v.size() == 3);
    assert(v[0] == 3);
    assert(v[1] == 2);
    assert(v[2] == 1);

    auto r = v.rbegin();
    assert(r != v.rend());
    ++r;
    assert(*r == 2);

    auto f = v.begin();
    assert(*f == 3);
    ++f;
    assert(f != v.end());

    v.reserve(10000);
    assert(v.size() == 3);
    assert(v.capacity() >= 10000);

    v.resize(2);
    assert(v.size() == 2);
    assert(v[0] == 3);
    assert(v[1] == 2);

    std::cout << "OK" << std::endl;
    return 0;
}

