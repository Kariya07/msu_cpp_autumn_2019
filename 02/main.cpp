#include "allocator.h"

int main() {
    int n = 20;
    LinearAllocator mass(n);
    std::cout << "maxSize = " << n << std::endl;
    std::cout << std::dec << "3: " << (int *) mass.alloc(3) << "\n0: " << (int *) mass.alloc(0) << "\n18: "
              << (int *) mass.alloc(18) << "\n17: " << (int *) mass.alloc(17) << "\n2: " << (int *) mass.alloc(2)
              << std::endl;
    mass.reset();
    std::cout << "2: " << (int *) mass.alloc(2) << "\n15: " << (int *) mass.alloc(15) << "\n3: "
              << (int *) mass.alloc(3) << std::endl;
    return 0;
}
