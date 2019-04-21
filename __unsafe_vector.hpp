/*
    author dzhiblavi
 */

#ifndef __unsafe_vector_hpp
#define __unsafe_vector_hpp

#include <cstddef>
#include <type_traits>
#include <cstring>

namespace __unsafe {
    template<typename T,
            typename = typename std::enable_if<std::is_integral<T>::value>::type>
    class vector {
    public:
        using value_type = T;
        using reference = T &;
        using const_reference = T const &;
        using pointer = T *;
        using const_pointer = T const *;

    private:
        pointer _data = nullptr;
        size_t _size = 0;
        size_t _capacity = 0;

        void _reallocate(size_t rls) {
            _data = static_cast<T *> (realloc(_data, sizeof (T) * rls));
            _capacity = rls;
        }

        void _push_back_fast(const_reference x) {
            _data[_size] = x;
        }

        void _push_back_slow(const_reference x) {
            size_t new_capacity = _capacity ? _capacity << 1 : 8;
            _reallocate(new_capacity);
            _data[_size] = x;
        }

        void _internal_resize_fast(size_t size) {
            memset(_data + _size, 0, sizeof(T) * (size - _size));
        }

        void _internal_resize_slow(size_t size) {
            size_t cap = _capacity;
            _reallocate(size);
            memset(_data + cap, 0, sizeof(T) * (size - cap));
        }

        void _internal_resize_less(size_t size) noexcept {
            memset(_data + size, 0, sizeof(T) * (_size - size));
        }

        void _internal_resize_greater(size_t size) {
            if (size <= _capacity)
                _internal_resize_fast(size);
            else
                _internal_resize_slow(size);
        }

    public:
        vector() noexcept = default;

        explicit vector(size_t initial_size)
        : _data((T *) malloc(sizeof(T) * initial_size)),
          _size(initial_size),
          _capacity(initial_size) {
            memset(_data, 0, sizeof(T) * initial_size);
        }

        ~vector() noexcept {
            free(_data);
        }

        vector(vector &&v) noexcept {
            swap(v);
        }

        vector(vector const &v) :
          _data((T*) malloc(sizeof(T) * v._size)),
          _size(v._size),
          _capacity(v._size) {
            memcpy(_data, v._data, sizeof(T) * v._size);
        }

        void swap(vector &v) {
            std::swap(_data, v._data);
            std::swap(_size, v._size);
            std::swap(_capacity, v._capacity);
        }

        vector &operator=(vector &&v) {
            swap(v);
            return *this;
        }

        vector &operator=(vector const &v) {
            if (&v == this)
                return *this;
            if (v._size > _capacity)
                _reallocate(v._size);
            memcpy(_data, v._data, sizeof (T) * v._size);
            if (_size > v._size)
                memset(_data + v._size, 0, sizeof(T) * (_size - v._size));
            _size = v._size;
            return *this;
        }

        const_reference operator[](size_t i) const noexcept {
            return _data[i];
        }

        reference operator[](size_t i) noexcept {
            return _data[i];
        }

        const_reference back() const noexcept {
            return _data[_size - 1];
        }

        reference back() noexcept {
            return _data[_size - 1];
        }

        const_reference front() const noexcept {
            return _data[0];
        }

        reference front() noexcept {
            return _data[0];
        }

        pointer data() noexcept {
            return _data;
        }

        const_pointer data() const noexcept {
            return _data;
        }

        size_t size() const noexcept {
            return _size;
        }

        bool empty() const noexcept {
            return !_size;
        }

        size_t capacity() const noexcept {
            return _capacity;
        }

        void pop_back() noexcept {
            --_size;
            _data[_size] = 0;
        }

        void push_back(const_reference x) {
            if (_size < _capacity)
                _push_back_fast(x);
            else
                _push_back_slow(x);
            ++_size;
        }

        void resize(size_t size) {
            if (size < _size)
                _internal_resize_less(size);
            else
                _internal_resize_greater(size);
            _size = size;
        }
    };
}

#endif /* __unsafe_vector.hpp */
