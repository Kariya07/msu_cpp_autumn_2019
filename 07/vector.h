#include <iostream>
#include <cassert>
#include <memory>
#include <vector>

template<class T>
class Allocator {
    using value_type = T;
    using pointer = T *;
    using size_type = size_t;
    pointer current_ptr;
    size_type size;
public:
    pointer allocate(size_type count) {
        current_ptr = (pointer) malloc(sizeof(T) * count);
        size = count;
        return current_ptr;
    }

    void deallocate(pointer ptr) {
        free(ptr);
    }

    void construct(pointer ptr) {
        ptr->value_type();
    }

    void destroy(pointer ptr) {
        ptr->~value_type();
    }

    void resize(size_type newSize) {
        if (size < newSize) {
            auto newData = (pointer) malloc(sizeof(T) * newSize);
            for (size_type i = 0; i < size; i++) {
                newData[i] = current_ptr[i];
                destroy(current_ptr + i);
            }
            free(current_ptr);
            current_ptr = newData;
            size = newSize;
            return;
        }
        assert(!"not implemented yet");
    }

    size_t max_size() const noexcept {
        return size;
    }

    pointer get() {
        return current_ptr;
    }

    const pointer cget() const {
        return current_ptr;
    }
};

template<class T>
class Iterator : public std::iterator<std::forward_iterator_tag, T> {
    T *ptr_;
public:
    explicit Iterator(T *ptr) : ptr_(ptr) {}

    bool operator==(const Iterator<T> &other) const {
        return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator<T> &other) const {
        return !(*this == other);
    }

    T &operator*() const {
        return *ptr_;
    }

    Iterator &operator++() {
        ++ptr_;
        return *this;
    }

    Iterator &operator--() {
        --ptr_;
        return *this;
    }
};

template<class T, class Alloc = Allocator<T>>
class Vector {
public:
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_iterator = const Iterator<T>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using size_type = size_t;
    using value_type = T;
    using pointer = T *;
    using reference = T &;
    using const_reference = const T &;
    using allocator_type = Alloc;

    size_type N;
    allocator_type _data;
    pointer current_ptr;

    iterator begin() noexcept {
        return iterator(current_ptr);
    }

    const_iterator cbegin() const noexcept {
        return iterator(_data.cget());
    }

    iterator end() noexcept {
        return iterator(current_ptr + N);
    }

    const_iterator cend() const noexcept {
        return iterator(_data.cget() + N);
    }

    explicit Vector(size_type count = 1024) {
        current_ptr = _data.allocate(count);
        N = 0;
    }

    Vector(size_type count, const value_type &defaultValue) {
        current_ptr = _data.allocate(count);
        for (size_type i = 0; i < count; i++) {
            current_ptr[i] = defaultValue;
        }
        N = count;
    }

    Vector(std::initializer_list<value_type> init) : N(init.size()) {
        current_ptr = _data.allocate(init.size());
        size_t i = 0;
        auto current = init.begin();
        const auto end = init.end();
        while (current != end) {
            current_ptr[i++] = *current++;
        }
    }

    ~Vector() {
        for (size_type i = 0; i < N; i++) {
            _data.destroy(current_ptr + i);
        }
        _data.deallocate(current_ptr);
    }

    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const noexcept {
        return reverse_iterator(end());
    }

    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    const_reverse_iterator crend() const noexcept {
        return reverse_iterator(begin());
    }

    void push_back(value_type &&value) {
        if (N == _data.max_size()) {
            _data.resize(2 * N);
            current_ptr = _data.get();
        }
        current_ptr[N++] = value;
    }

    void push_back(const value_type &value) {
        if (N == _data.max_size()) {
            _data.resize(2 * N);
            current_ptr = _data.get();
        }
        current_ptr[N++] = value;
    }

    void pop_back() {
        if (N != 0) {
            N--;
            _data.destroy(current_ptr + N);
        } else {
            throw std::out_of_range("pop_back");
        }
    }

    bool empty() const {
        return cbegin() == cend();
    }

    size_type size() const {
        return N;
    }

    void reserve(size_type count) {
        size_type my_capacity = _data.max_size();
        if (my_capacity < count) {
            _data.resize(count);
            current_ptr = _data.get();
        }
    }

    size_type capacity() const noexcept {
        return _data.max_size();
    }

    void resize(size_type newsize) {
        if (newsize < N) {
            for (size_type i = newsize; i < N; i++) {
                _data.destroy(current_ptr + i);
            }
            N = newsize;
        } else {
            if (newsize > N) {
                size_type my_capacity = _data.max_size();
                if (newsize > my_capacity) {
                    _data.resize(newsize);
                    current_ptr = _data.get();
                }
                while (N < newsize) {
                    current_ptr[N++] = T();
                }
            }
        }
    }

    void resize(size_type newsize, const value_type &defaultValue) {
        if (newsize < N) {
            N = newsize;
            for (size_type i = newsize; i < N; i++) {
                _data.destroy(current_ptr + i);
            }
        } else {
            if (newsize > N) {
                size_type my_capacity = _data.max_size();
                if (newsize > my_capacity) {
                    _data.resize(newsize);
                    current_ptr = _data.get();
                }
                while (N < newsize) {
                    current_ptr[N++] = defaultValue;
                }
            }
        }
    }

    void clear() noexcept {
        while (N > 0) {
            pop_back();
        }
    }

    const_reference operator[](size_type pos) {
        if (pos >= N)
            throw std::out_of_range("Out of range");
        return *(current_ptr + pos);
    }
};
