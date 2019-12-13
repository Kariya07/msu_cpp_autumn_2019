template<class T>
class Allocator {
    using value_type = T;
    using pointer = T *;
    using size_type = size_t;
    size_type size;
public:
    pointer allocate(size_type count) {
        size = count;
        return (pointer) malloc(sizeof(value_type) * count);
    }

    void deallocate(pointer ptr) {
        free(ptr);
    }

    template <class... Args>
    void construct(pointer ptr, Args&&... args) {
        new(ptr) value_type(std::forward <Args> (args)...);
    }

    void destroy(pointer ptr) {
        ptr->~value_type();
    }

    size_t max_size() const noexcept {
        return size;
    }
};
