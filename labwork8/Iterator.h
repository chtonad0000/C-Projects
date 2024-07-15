#pragma once

#include <iostream>

template<typename T>
class CircularBufferIterator {
public:
    using It = CircularBufferIterator;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&;

    CircularBufferIterator(T* buffer, size_t buffer_size, size_t current_index) : buffer_(buffer),
                                                                                  buffer_size_(buffer_size),
                                                                                  current_index_(current_index) {
    }

    reference operator*() const {
        return buffer_[current_index_];
    }

    value_type* operator->() const {
        return &(buffer_[current_index_]);
    }

    reference operator[](int n) const {
        return *(*this + n);
    }

    It& operator++() {
        current_index_ = (current_index_ + 1) % (buffer_size_);

        return *this;
    }

    It operator++(int) {
        CircularBufferIterator temp = *this;
        ++(*this);

        return temp;
    }

    It& operator--() {
        if (current_index_ == 0) {
            current_index_ = buffer_size_;
        } else {
            --current_index_;
        }

        return *this;
    }

    It operator--(int) {
        CircularBufferIterator temp = *this;
        --(*this);

        return temp;
    }

    It& operator+=(long long int n) {
        current_index_ = (current_index_ + n) % (buffer_size_);

        return *this;
    }

    It& operator-=(long long int n) {
        if (n > static_cast<long long int>(current_index_)) {
            current_index_ = buffer_size_ - (n - current_index_);
        } else {
            current_index_ -= n;
        }

        return *this;
    }

    It operator+(long long int n) const {
        CircularBufferIterator result(*this);
        result += n;

        return result;
    }

    It operator+(const CircularBufferIterator& other) const {
        CircularBufferIterator result(*this);
        result += (other - *this);

        return result;
    }

    It operator-(long long int n) const {
        CircularBufferIterator result(*this);
        result -= n;

        return result;
    }

    difference_type operator-(const CircularBufferIterator& other) const {
        long long int diff = static_cast<long long>(current_index_) - static_cast<long long int>(other.current_index_);
        if (diff < 0) {
            diff += buffer_size_ + 1;
        }

        return diff;
    }

    bool operator==(const CircularBufferIterator& other) const {
        return current_index_ == other.current_index_;
    }

    bool operator!=(const CircularBufferIterator& other) const {
        return !(*this == other);
    }

    bool operator<(const CircularBufferIterator& other) const {
        long long int diff =
                static_cast<long long int>(current_index_) - static_cast<long long int>(other.current_index_);
        if (diff < 0) {
            diff += buffer_size_;
        }

        return diff < 0;
    }

    bool operator>(const CircularBufferIterator& other) const {
        return other < *this;
    }

    bool operator<=(const CircularBufferIterator& other) const {
        return !(other < *this);
    }

    bool operator>=(const CircularBufferIterator& other) const {
        return !(*this < other);
    }

    int long long current_index_;
    size_t buffer_size_;
private:
    T* buffer_;

};

template<typename T>
class CircularBufferConstIterator {
public:
    using It = CircularBufferConstIterator;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&;

    CircularBufferConstIterator(T* buffer, size_t buffer_size, size_t current_index) : buffer_(buffer),
                                                                                       buffer_size_(buffer_size),
                                                                                       current_index_(current_index) {
    }

    const reference operator*() const {
        return buffer_[current_index_];
    }

    const value_type* operator->() const {
        return &(buffer_[current_index_]);
    }

    const reference operator[](int n) const {
        return *(*this + n);
    }

    It& operator++() {
        current_index_ = (current_index_ + 1) % (buffer_size_);
        return *this;
    }

    It operator++(int) {
        CircularBufferIterator temp = *this;
        ++(*this);

        return temp;
    }

    It& operator--() {
        if (current_index_ == 0) {
            current_index_ = buffer_size_;
        } else {
            --current_index_;
        }
        return *this;
    }

    It operator--(int) {
        CircularBufferIterator temp = *this;
        --(*this);

        return temp;
    }

    It& operator+=(int n) {
        current_index_ = (current_index_ + n) % buffer_size_;
        return *this;
    }

    It& operator-=(long long int n) {
        if (n > static_cast<long long int>(current_index_)) {
            current_index_ = buffer_size_ - (n - current_index_);
        } else {
            current_index_ -= n;
        }
        return *this;
    }

    It operator+(long long int n) const {
        CircularBufferConstIterator result(*this);
        result += n;
        return result;
    }

    It operator+(const CircularBufferConstIterator& other) const {
        CircularBufferConstIterator result(*this);
        result += (other - *this);
        return result;
    }

    It operator-(long long int n) const {
        CircularBufferConstIterator result(*this);
        result -= n;
        return result;
    }

    difference_type operator-(const CircularBufferConstIterator& other) const {
        long long int diff = static_cast<long long>(current_index_) - static_cast<long long int>(other.current_index_);
        if (diff < 0) {
            diff += buffer_size_ + 1;
        }
        return diff;
    }

    bool operator==(const CircularBufferConstIterator& other) const {
        return current_index_ == other.current_index_;
    }

    bool operator!=(const CircularBufferConstIterator& other) const {
        return !(*this == other);
    }

    bool operator<(const CircularBufferConstIterator& other) const {
        long long int diff =
                static_cast<long long int>(current_index_) - static_cast<long long int>(other.current_index_);
        if (diff < 0) {
            diff += buffer_size_;
        }
        return diff < 0;
    }

    bool operator>(const CircularBufferConstIterator& other) const {
        return other < *this;
    }

    bool operator<=(const CircularBufferConstIterator& other) const {
        return !(other < *this);
    }

    bool operator>=(const CircularBufferConstIterator& other) const {
        return !(*this < other);
    }


    int long long current_index_;
    size_t buffer_size_;
private:
    T* buffer_;
};