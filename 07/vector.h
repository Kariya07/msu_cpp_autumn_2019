#include <iostream>
#include <cassert>

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

    void buf_resize(size_type newSize) {
        if (_data.max_size() < newSize) {
            pointer newData = _data.allocate(newSize);;
            for (size_type i = 0; i < N; i++) {
                _data.construct(newData + i, current_ptr[i]);
                _data.destroy(current_ptr + i);
            }
            _data.deallocate(current_ptr);
            current_ptr = newData;
            return;
        }
        assert(!"not implemented yet");
    }

    iterator begin() noexcept {
        return iterator(current_ptr);
    }

    const_iterator cbegin() const noexcept {
        return iterator(current_ptr);
    }

    iterator end() noexcept {
        return iterator(current_ptr + N);
    }

    const_iterator cend() const noexcept {
        return iterator(current_ptr + N);
    }

    explicit Vector(size_type count = 1024) {
        current_ptr = _data.allocate(count);
        N = 0;
    }

    Vector(size_type count, const value_type &defaultValue) {
        current_ptr = _data.allocate(count);
        for (size_type i = 0; i < count; i++) {
            _data.construct(current_ptr + i, std::move(defaultValue));
        }
        N = count;
    }

    Vector(std::initializer_list<value_type> init) : N(init.size()) {
        current_ptr = _data.allocate(init.size());
        size_t i = 0;
        auto current = init.begin();
        const auto end = init.end();
        while (current != end) {
            _data.construct(current_ptr + i, std::move(*current++));
            i++;
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
            buf_resize(2 * N);
        }
        _data.construct(current_ptr + N, std::move(value));
        N++;
    }

    void push_back(const value_type &value) {
        if (N == _data.max_size()) {
            buf_resize(2 * N);
        }
        _data.construct(current_ptr + N, std::move(value));
        N++;
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
            buf_resize(count);
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
                    buf_resize(newsize);
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
                    buf_resize(newsize);
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
