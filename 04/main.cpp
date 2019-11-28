#include <iostream>
#include <cassert>
#include <limits>
#include "bigint.h"

int main() {
    try {
        BigInt x = 3;
        assert(x == 3);
        BigInt y = x;
        assert(y == 3);
        BigInt z;
        assert(z == 0);
        assert(BigInt(-10) == -10);
        assert(x == y);
        assert(y == x);
        assert(x != z);
        assert(z != x);
        z = y;
        assert(z == 3);
        x = 100;
        assert(x == 100);
        assert(!(x < x));
        assert(!(x > x));
        assert(x < 200);
        assert(BigInt(50) < x);
        assert(BigInt(200) > x);
        assert(x > BigInt(50));
        assert(BigInt(-200) > BigInt(-500));
        assert(BigInt(-500) <= BigInt(-200));
        assert(x >= BigInt(50));
        assert(BigInt(0) == -BigInt(0));
        assert(BigInt(-10) + BigInt(10) == 0);
        assert(BigInt(-10) - BigInt(10) == (-20));
        assert(BigInt(10) - BigInt(-10) == 20);
        assert(BigInt(99) - BigInt(100) == -1);
        assert(BigInt(10) - BigInt(11) == -1);
        const BigInt big1 = std::numeric_limits<int64_t>::max();
        assert(big1 == 9223372036854775807);
        const BigInt big2 = big1 + big1;
        const BigInt big3 = big2 - big1;
        assert(big3 == 9223372036854775807);
        std::cout << "OK\n";
    }
    catch (std::runtime_error &err) {
        std::cout << err.what() << std::endl;
    }
    return 0;
}
