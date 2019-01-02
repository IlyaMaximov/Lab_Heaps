//
// Created by User on 06.11.2018.
//

#ifndef LAB1_VECTOR_H
#define LAB1_VECTOR_H

#include <stdarg.h>
#include <cassert>
#include <stdexcept>

template <class T>
class Vector {
public:

    explicit Vector() {
        head_ = nullptr;
        memory_ = size_ = 0;
    }

    explicit Vector(int n, T value) {
        if (n < 0) {
            throw std::logic_error("incorrect array creation");
        }
        size_ = n;
        memory_ = 1;
        while (memory_ < size_) {
            memory_ <<= 1;
        }
        head_ = new T[memory_];
        for (int i = 0; i < size_; ++i) {
            head_[i] = value;
        }
    }

    explicit Vector(int n, T& sample) {
        if (n < 0) {
            throw std::logic_error("incorrect array creation");
        }
        if (n == 0) {
            head_ = nullptr;
            memory_ = size_ = 0;
            return ;
        }
        size_ = n;
        memory_ = 1;
        while (memory_ < size_) {
            memory_ <<= 1;
        }
        head_ = new T[memory_];
        for (int i = 0; i < size_; ++i) {
            head_[i] = sample;
        }
    }

    explicit Vector(int n, ...) {
         if (n < 0) {
             throw std::logic_error("incorrect array creation");
         }
        if (n == 0) {
            head_ = nullptr;
            memory_ = size_ = 0;
            return ;
        }
        size_ = n;
        memory_ = 1;
        while (memory_ < size_) {
            memory_ <<= 1;
        }
        head_ = new T[memory_];
        va_list factor;
        va_start(factor, n);
        for (int i = 0; i < size_; ++i) {
            head_[i] = va_arg(factor, T);
        }
        va_end(factor);
    }

    ~Vector() {
        memory_ = size_ = 0;
        delete[] head_;
    }

    Vector(const Vector<T>& a) {
        if (a.size_ == 0)
        {
            head_ = nullptr;
            memory_ = size_ = 0;
        }
        size_ = a.size_;
        memory_ = 1;
        while (memory_ < size_) {
            memory_ <<= 1;
        }
        head_ = new T[memory_];
        for (int i = 0; i < size_; ++i) {
            head_[i] = a[i];
        }
    }

    Vector& operator = (const Vector<T>& a) {
        if (a.size_ == 0)
        {
            head_ = nullptr;
            memory_ = size_ = 0;
        }
        size_ = a.size_;
        memory_ = 1;
        while (memory_ < size_) {
            memory_ <<= 1;
        }
        head_ = new T[memory_];
        for (int i = 0; i < size_; ++i) {
            head_[i] = a[i];
        }
        return (*this);
    }

    void Clear() {
        memory_ = size_ = 0;
        delete[] head_;
        head_ = nullptr;
    }

    void Push_Back(const T& new_value) {
        if (size_ < memory_){
            head_[size_] = new_value;
            size_++;
        }
        else {
            if (memory_ == 0){
                head_ = new T[1];
                head_[0] = new_value;
                size_ = memory_ = 1;
            }
            else {
                T* head_tmp_ = new T[memory_ * 2];
                for (int i = 0; i < size_; ++i) {
                    head_tmp_[i] = head_[i];
                }
                T * tmp_ = head_;
                head_ = head_tmp_;
                delete (tmp_);
                head_[size_] = new_value;
                size_++;
                memory_ *= 2;
            }
        }
    }

    void Pop_Back() {
        if (size_ <= 0)
            throw std::logic_error("deleting from array of size 0");
        assert(size_ >= memory_ / 4);
        size_--;
        if (size_ <= memory_ / 4){
            T* head_tmp_ = new T[memory_ / 2];
            for (int i = 0; i < size_; ++i) {
                head_tmp_[i] = head_[i];
            }
            T * tmp_ = head_;
            head_ = head_tmp_;
            delete (tmp_);
            memory_ /= 2;
        }
    }

    T& operator [](int i) const {
        if (i < 0 || i >= size_) {
            throw std::out_of_range("accessing a nonexistent array element");
        }
        return head_[i];
    }

    bool is_empty() const {
        return (size_ == 0);
    }

    size_t size() const {
        return size_;
    }


private:

    size_t memory() const {
        return memory_;
    }

    T* head_;
    size_t memory_, size_;

};

#endif //LAB1_VECTOR_H
