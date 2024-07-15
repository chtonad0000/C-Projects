#pragma once

#include <iterator> //для кастомных классов

template<typename T>
class XrangeIterator {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using reference = T&;
    using const_pointer = const T*;
    using const_reference = const T&;

    XrangeIterator(value_type current_number, value_type step, value_type end) : current_number_(current_number),
                                                                                 step_(step), end_(end) {
    };

    value_type operator*() const {
        return current_number_;
    }

    value_type* operator->() const {
        return &current_number_;
    }

    void operator++() {
        if (current_number_ + step_ >= end_) {
            current_number_ = end_;

            return;
        }
        current_number_ += step_;
    }

    XrangeIterator operator++(int) {
        XrangeIterator tmp = *this;
        ++(*this);

        return tmp;

    }

    bool operator==(const XrangeIterator& other) {
        return current_number_ == other.current_number_;
    }

    bool operator!=(const XrangeIterator& other) {
        return !(*this == other);
    }

private:
    value_type current_number_;
    value_type end_;
    value_type step_;
};

template<typename T>
class xrange {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = XrangeIterator<T>;
    using const_iterator = const XrangeIterator<T>;

    xrange(value_type end) : start_(value_type()), end_(end), step_(1) {
    };

    xrange(value_type start, value_type end) : start_(start), end_(end) {
    };

    xrange(value_type start, value_type end, value_type step) : start_(start), end_(end), step_(step) {
    };

    const_iterator begin() const {
        return XrangeIterator<T>(start_, step_, end_);
    }

    const_iterator end() const {
        return XrangeIterator<T>(end_, step_, end_);
    }

private:
    T start_;
    T end_;
    T step_;
};
