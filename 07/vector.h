#include <iostream>
#include <cassert>

#include "iterator.h"
#include "alloc.h"

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
    allocator_type _alloc;
    pointer current_ptr;

    void buf_resize(size_type newSize) {
        if (_alloc.max_size() < newSize) {
            pointer newData = _alloc.allocate(newSize);;
            for (size_type i = 0; i < _alloc.max_size(); i++) {
                _alloc.construct(newData + i, current_ptr[i]);
                _alloc.destroy(current_ptr + i);
            }
            _alloc.deallocate(current_ptr);
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
        current_ptr = _alloc.allocate(count);
        N = 0;
    }

    Vector(size_type count, const value_type &defaultValue) {
        current_ptr = _alloc.allocate(count);
        for (size_type i = 0; i < count; i++) {
            _alloc.construct(current_ptr + i, defaultValue);
        }
        N = count;
    }

    Vector(std::initializer_list<value_type> init) : N(init.size()) {
        current_ptr = _alloc.allocate(init.size());
        size_t i = 0;
        auto current = init.begin();
        const auto end = init.end();
        while (current != end) {
            _alloc.construct(current_ptr + i, std::move(*current++));
            i++;
        }
    }

    ~Vector() {
        for (size_type i = 0; i < N; i++) {
            _alloc.destroy(current_ptr + i);
        }
        _alloc.deallocate(current_ptr);
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
        if (N == _alloc.max_size()) {
            buf_resize(2 * N);
        }
        _alloc.construct(current_ptr + N, std::move(value));
        N++;
    }

    void push_back(const value_type &value) {
        if (N == _alloc.max_size()) {
            buf_resize(2 * N);
        }
        _alloc.construct(current_ptr + N, value);
        N++;
    }

    void pop_back() {
        if (N != 0) {
            N--;
            _alloc.destroy(current_ptr + N);
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
        size_type my_capacity = _alloc.max_size();
        if (my_capacity < count) {
            buf_resize(count);
        }
    }

    size_type capacity() const noexcept {
        return _alloc.max_size();
    }

    void resize(size_type newsize) {
        if (newsize < N) {
            for (size_type i = newsize; i < N; i++) {
                _alloc.destroy(current_ptr + i);
            }
            N = newsize;
        } else {
            if (newsize > N) {
                size_type my_capacity = _alloc.max_size();
                if (newsize > my_capacity) {
                    buf_resize(newsize);
                }
                while (N < newsize) {
                    _alloc.construct(current_ptr + N);
                    N++;
                }
            }
        }
    }

    void resize(size_type newsize, const value_type &defaultValue) {
        if (newsize < N) {
            N = newsize;
            for (size_type i = newsize; i < N; i++) {
                _alloc.destroy(current_ptr + i);
            }
        } else {
            if (newsize > N) {
                size_type my_capacity = _alloc.max_size();
                if (newsize > my_capacity) {
                    buf_resize(newsize);
                }
                while (N < newsize) {
                    _alloc.construct(current_ptr + N, defaultValue);
                    N++;
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
