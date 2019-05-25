//
// Created by ASUS on 23.05.2019.
//

#ifndef BIGINT_SHARED_PTR_H
#define BIGINT_SHARED_PTR_H

#include <cstddef>

template <typename T>
struct shared_ptr {
    struct Aux {
        explicit Aux(T *_ptr) {
            ptr = _ptr;
            count = 1;
        }
        ~Aux() {
            operator delete [] (ptr);
        }
        T *ptr;
        size_t count;
    };

    explicit shared_ptr(T *ptr = nullptr) {
        if (ptr == NULL) {
            aux = nullptr;
        } else {
            aux = new Aux(ptr);
        }
    }

    ~shared_ptr() {
        if (aux != nullptr) {
            subtr();
        }
    }

    shared_ptr(const shared_ptr& a) {
        if (a.aux == nullptr) {
            aux = nullptr;
        } else {
            aux = a.aux;
            aux->count++;
        }
    }

    shared_ptr &operator=(const shared_ptr & a) {
        if (aux == a.aux) {
            return *this;
        }
        if (aux != nullptr) {
            subtr();
        }
        if (a.aux == nullptr) {
            aux = nullptr;
        } else {
            aux = a.aux;
            aux->count++;
        }
        return *this;
    }

    T *get() const {
        return (aux == nullptr) ? nullptr : aux->ptr;
    }

    void reset(T *ptr = nullptr) {
        if (aux != nullptr) {
            subtr();
        }
        if (ptr == nullptr) {
            aux = nullptr;
        } else {
            aux = new Aux(ptr);
        }
    }

    T &operator*() const {
        return *(aux->ptr);
    }

    T *operator->() const {
        return (aux == nullptr) ? nullptr : aux->ptr;
    }

    T &operator[](size_t i) {
        return aux->ptr[i];
    }

    const T &operator[](size_t i) const {
        return aux->ptr[i];
    }

    bool only() {
        return (aux != nullptr && aux->count == 1);
    }

    void subtr() {
        aux->count--;
        if (aux->count == 0) {
            delete aux;
        }
    }

private:
    Aux *aux;
};


#endif //BIGINT_SHARED_PTR_H
