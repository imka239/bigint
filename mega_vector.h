//
// Created by ASUS on 25.05.2019.
//

#ifndef BIGINT_MEGA_VECTOR_H
#define BIGINT_MEGA_VECTOR_H

#include <cstddef>
#include "shared_ptr.h"

template<typename T>
class mega_vector {
    static const size_t max_sz = 2;
    size_t sz = 0;
    size_t all_sz = 0;

    T small[max_sz];
    shared_ptr<T> big;

    void convert(size_t allocated = 4) {
        all_sz = allocated;
        big.reset((T *) operator new[] (all_sz * sizeof(T)));
        for (size_t i = 0; i < sz; i++) {
            new(big.get() + i) T(small[i]);
        }
        for (size_t i = 0; i < sz; i++) {
            small[i].~T();
        }
    }

    void remake() {
        T *new_big = (T *) operator new[] (all_sz * sizeof(T));
        for (size_t i = 0; i < sz; i++) {
            new(new_big + i) T (big[i]);
        }
        big.reset(new_big);
    }

public:
    mega_vector() : sz(0), all_sz(0) {}

    mega_vector(const mega_vector<T> &that) :sz(that.sz), all_sz(that.all_sz), big(that.big) {
        if (all_sz == 0) {
            for(size_t i = 0; i < sz; i++) {
                new(small + i) T (that.small[i]);
            }
        }
    }

    ~mega_vector() {
        if (big.get() != nullptr) {
            for (size_t i = 0; i < sz; i++) {
                big[i].~T();
            }
        } else {
            for (size_t i = 0; i < sz; i++) {
                small[i].~T();
            }
        }
    }

    mega_vector &operator=(const mega_vector &that) {
        if (that.big.get() != nullptr) {
            if (big.get() == nullptr) {
                for (size_t i = 0; i < sz; i++) {
                    small[i].~T();
                }
            }
            big = that.big;
        } else {
            if (big.get() == nullptr) {
                for (size_t i = 0; i < sz; i++) {
                    small[i].~T();
                }
            } else {
                for (size_t i = 0; i < sz; i++) {
                    big[i].~T();
                }
            }
            for (size_t i = 0; i < that.sz; i++) {
                new(small + i) T (that.small[i]);
            }
            big.reset();
        }
        sz = that.sz;
        all_sz = that.all_sz;
        return (*this);
    }

    void push_back(T value) {
        if (sz == max_sz && big.get() == nullptr) {
            convert();
        }
        if (big.get() == nullptr) {
            new(small + sz) T (value);
        } else {
            if (all_sz == sz) {
                all_sz *= 2;
                remake();
            }
            new(big.get() + sz) T (value);
        }
        ++sz;
    }

    void reserve(size_t allocated) {
        if (max_sz < allocated) {
            if (big.get() == nullptr) {
                all_sz = 4;
                while (all_sz < allocated) {
                    all_sz *= 2;
                }
                convert(all_sz);
            } else {
                while (all_sz < allocated) {
                    all_sz *= 2;
                }
                remake();
            }
        }
    }

    void resize(size_t new_sz, const T &base = T()) {
        if (new_sz <= sz) {
            sz = new_sz;
            if (big.get() != nullptr && sz <= max_sz) {
                for (size_t i = 0; i < sz; i++) {
                    new(small + i) T (big[i]);
                }
                all_sz = 0;
                big.reset();
            }
        } else {
            if (max_sz < new_sz) {
                if (big.get() == nullptr) {
                    all_sz = 4;
                    while (all_sz < new_sz) {
                        all_sz *= 2;
                    }
                    convert(all_sz);
                } else {
                    if (all_sz < new_sz) {
                        while (all_sz < new_sz) {
                            all_sz *= 2;
                        }
                        remake();
                    }
                }
            }
            while (sz < new_sz) {
                push_back(base);
            }
        }
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    void make_new() {
        if (big.get() != nullptr && !big.only()) {
            remake();
        }
    }


    T &operator[](size_t i) {
        if (big.get() == nullptr) {
            return small[i];
        }
        return big[i];
    }

    const T &operator[](size_t i) const {
        if (big.get() == nullptr) {
            return small[i];
        }
        return big[i];
    }
};


#endif //BIGINT_MEGA_VECTOR_H
