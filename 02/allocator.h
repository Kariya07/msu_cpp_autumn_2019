#include <iostream>
#include <cstdlib>

class LinearAllocator {
private:
    char *memory;
    char *send_mem;
    size_t maxlength;
    size_t offset;
public:
    LinearAllocator(size_t maxSize) : maxlength(maxSize), offset(0), send_mem(nullptr) {
        memory = (char *) malloc(maxSize);
    }

    ~LinearAllocator() {
        delete (memory);
    }

    char *alloc(size_t size);

    void reset();
};

char *LinearAllocator::alloc(size_t size) {
    send_mem = nullptr;
    if ((offset + size) <= maxlength && size != 0) {
        send_mem = memory + offset;
        offset += size;
    }
    return send_mem;
}

void LinearAllocator::reset() {
    offset = 0;
}
