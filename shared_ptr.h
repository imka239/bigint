//
// Created by ASUS on 23.05.2019.
//

#ifndef BIGINT_SHARED_PTR_H
#define BIGINT_SHARED_PTR_H

#include <cstddef>

template <typename T>
class shared_ptr {
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
        if (*ptr == 0) {
            aux = nullptr;
        } else {
            aux = new Aux(ptr);
        }
    }

    ~shared_ptr() {
        if (!nulptr()) {
            subtr();
        }
    }

    shared_ptr(shared_ptr& a) {
        if (a.nulptr()) {
            aux = nullptr;
        } else {
            aux = a.aux;
            aux->count++;
        }
    }

    shared_ptr &operator=(shared_ptr & a) {
        if (aux == a.aux) {
            return *this;
        }
        if (!nulptr()) {
            subtr();
        }
        if (a.nulptr()) {
            aux = nullptr;
        } else {
            aux = a.aux;
            aux->count++;
        }
        return *this;
    }

    T *get() const {
        return (nulptr()) ? nullptr : aux->ptr;
    }

    void reset(T *ptr = nullptr) {
        if (!nulptr()) {
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
        return (nulptr()) ? nullptr : aux->ptr;
    }

    T &operator[](size_t i) const {
        return *(aux->ptr + i);
    }

    bool only() {
        return (!nulptr() && aux->count == 1);
    }

    void subtr() {
        aux->count--;
        if (aux->count == 0) {
            delete aux;
        }
    }

    bool nulptr() {
        return aux == nullptr;
    }

private:
    Aux *aux;
};


#endif //BIGINT_SHARED_PTR_H
