/*
    author dzhiblavi
 */

#ifndef vector_hpp
#define vector_hpp

#include <cstddef>
#include <type_traits>
#include <cstring>

template<typename T>
void _destruct(T *_data, size_t size,
               typename std::enable_if<std::is_trivially_destructible<T>::value>::type * = 0) noexcept {}

template<typename T>
void _destruct(T *_data, size_t size,
               typename std::enable_if<!std::is_trivially_destructible<T>::value>::type * = 0) noexcept {
    for (size_t i = 0; i < size; ++i)
        _data[i].~T();
}

template<typename T>
void _copy_construct(T *_dst, T const *src, size_t size,
                     typename std::enable_if<std::is_trivially_copy_constructible<T>::value>::type * = 0) noexcept {
    memcpy(_dst, src, size * sizeof(T));
}

template<typename T>
void _copy_construct(T *_dst, T const *src, size_t size,
                     typename std::enable_if<!std::is_trivially_copy_constructible<T>::value>::type * = 0) {
    size_t i = 0;
    try {
        for (; i < size; ++i)
            new(_dst + i) T(src[i]);
    } catch (...) {
        _destruct(_dst, i);
        throw;
    }
}

template<typename T>
void _default_construct(T *_dst, size_t size,
        typename std::enable_if<std::is_trivially_constructible<T>::value>::type * = 0){
    memset(_dst, 0, size * sizeof(T));
}

template<typename T>
void _default_construct(T *_dst, size_t size,
        typename std::enable_if<std::is_default_constructible<T>::value
              & !std::is_trivially_constructible<T>::value>::type * = 0) {
    size_t i = 0;
    try {
        for (; i < size; ++i)
            new(_dst + i) T();
    } catch (...) {
        _destruct(_dst, i);
        throw;
    }
}

template<typename T>
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

    pointer _allocate_new_zone(const_pointer _src, size_t size, size_t alloc) {
        pointer _alloc_data = static_cast<T *> (operator new(alloc * sizeof(T)));
        if (!_src)
            return _alloc_data;
        try {
            _copy_construct(_alloc_data, _src, size);
        } catch (...) {
            operator delete(_alloc_data);
            throw;
        }
        return _alloc_data;
    }

    void _push_back_fast(const_reference x) {
        new(_data + _size) T(x);
    }

    void _push_back_slow(const_reference x) {
        size_t new_capacity = _capacity ? _capacity << 1 : 8;
        pointer _alloc_data = _allocate_new_zone(_data, _size, new_capacity);
        try {
            new(_alloc_data + _size) T(x);
        } catch (...) {
            _destruct(_alloc_data, _size);
            operator delete(_alloc_data);
            throw;
        }
        _remove_data();
        _data = _alloc_data;
        _capacity = new_capacity;
    }

    void _internal_resize_fast(size_t size) {
        _default_construct(_data + _size, size - _size);
    }

    void _internal_resize_slow(size_t size) {
        pointer _alloc_data = _allocate_new_zone(_data, _size, size);
        try {
            _default_construct(_alloc_data + _size, size - _size);
        } catch (...) {
            _destruct(_alloc_data, _size);
            operator delete(_alloc_data);
            throw;
        }
        _remove_data();
        _data = _alloc_data;
        _capacity = size;
    }

    void _internal_resize_less(size_t size) noexcept {
        _destruct(_data + size, _size - size);
    }

    void _internal_resize_greater(size_t size) {
        if (size <= _capacity)
            _internal_resize_fast(size);
        else
            _internal_resize_slow(size);
    }

    void _remove_data() noexcept {
        if (!_data)
            return;
        _destruct(_data, _size);
        operator delete(_data);
    }

public:
    vector() noexcept = default;

    explicit vector(size_t initial_size) {
        pointer _alloc_data = static_cast<T *> (operator new(initial_size * sizeof(T)));
        try {
            _default_construct(_alloc_data, initial_size);
        } catch (...) {
            operator delete(_alloc_data);
            throw;
        }
        _data = _alloc_data;
        _size = _capacity = initial_size;
    }

    ~vector() noexcept {
        _remove_data();
    }

    vector(vector &&v) noexcept {
        swap(v);
    }

    vector(vector const &v) {
        pointer _alloc_data = _allocate_new_zone(v._data, v._size, v._size);
        _data = _alloc_data;
        _size = _capacity = v._size;
    }

    void swap(vector &v) {
        std::swap(_data, v._data);
        std::swap(_size, v._size);
        std::swap(_capacity, v._capacity);
    }

    vector &operator=(vector&& v) {
        swap(v);
        return *this;
    }

    vector &operator=(vector const &v) {
        if (&v == this)
            return *this;
        pointer _alloc_data = _allocate_new_zone(v._data, v._size, v._size);
        _remove_data();
        _data = _alloc_data;
        _size = _capacity = v._size;
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
        _destruct(_data + _size, 1);
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

#endif /* vector.hpp */
