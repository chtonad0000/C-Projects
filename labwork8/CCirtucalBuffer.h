#pragma once

#include "Iterator.h"

#include <memory>

template<typename T, typename Allocator=std::allocator<T>>
class CCirtucalBuffer {
public:
    using traits = std::allocator_traits<Allocator>;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using iterator = CircularBufferIterator<T>;
    using const_iterator = CircularBufferConstIterator<T>;
    using difference_type = signed int;

    CCirtucalBuffer() : capacity_(0) {
    };

    CCirtucalBuffer(int capacity) : buffer_(traits::allocate(allocator_, capacity)), capacity_(capacity) {
        ConstructBuffer();
    }

    CCirtucalBuffer(int capacity, value_type value) {
        buffer_ = traits::allocate(allocator_, capacity);
        capacity_ = capacity;
        ConstructBuffer();
        std::fill(buffer_, buffer_ + capacity_, value);
        size_ = capacity;
        tail_ = capacity_;
    }

    CCirtucalBuffer(iterator first, iterator second) {
        if ((second - 1) - first < 0) {
            capacity_ = first.buffer_size_ - abs((second - 1) - first) + 1;
        } else {
            capacity_ = ((second - 1) - first) + 1;
        }
        buffer_ = traits::allocate(allocator_, capacity_);
        ConstructBuffer();
        auto j = begin();
        for (auto i = first; i != second - 1; ++i) {
            ++tail_;
            *j = *i;
            ++size_;
            j = iterator(buffer_, size_, j.current_index_ + 1);
        }
        *(j) = *(second - 1);
        ++size_;
        ++tail_;
    }

    CCirtucalBuffer(CCirtucalBuffer<value_type>& other) {
        *this = CCirtucalBuffer(other.begin(), other.end());
    }

    CCirtucalBuffer(const std::initializer_list<value_type>& il) {
        capacity_ = il.size();
        size_ = il.size();
        tail_ = il.size();
        buffer_ = traits::allocate(allocator_, capacity_);
        ConstructBuffer();
        std::copy(il.begin(), il.end(), buffer_);
    }

    ~CCirtucalBuffer() {
        DestructBuffer();
        traits::deallocate(allocator_, buffer_, capacity_);
    }

    reference operator[](size_t index) {
        if (head_ != 0 && tail_ <= head_ && size_ < capacity_) {
            return buffer_[(head_ + index) % (size_ + 1)];
        } else {
            return buffer_[(index + head_) % size_];
        }
    }

    iterator begin() {
        return iterator(buffer_, size_, head_);
    }

    iterator end() {
        return iterator(buffer_, size_, tail_);
    }

    const_iterator cbegin() {
        return const_iterator(buffer_, size_, head_);
    }

    const_iterator cend() {
        return const_iterator(buffer_, size_, tail_);
    }

    CCirtucalBuffer& operator=(const CCirtucalBuffer<value_type>& other) {
        if (this != &other) {
            DestructBuffer();
            traits::deallocate(allocator_, buffer_, capacity_);
            capacity_ = other.capacity_;
            size_ = other.size_;
            buffer_ = traits::allocate(allocator_, capacity_);
            ConstructBuffer();
            std::copy(other.buffer_, other.buffer_ + capacity_, buffer_);
        }

        return *this;
    }

    bool operator==(CCirtucalBuffer<value_type>& other) {
        if (size_ != other.size_) {
            return false;
        }
        auto j = other.begin();
        for (auto i = begin(); i != (end()) - 1; ++i) {
            if (*i != *j) {
                return false;
            }
            ++j;
        }
        if (*j != *((end()) - 1)) {
            return false;
        }

        return true;
    }

    bool operator!=(CCirtucalBuffer<value_type>& other) {
        return !(*this == other);
    }

    void swap(CCirtucalBuffer<value_type>& other) {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
    }

    size_t size() {
        return size_;
    }

    size_t max_size() const {
        const auto alloc_max_size = std::allocator_traits<Allocator>::max_size(allocator_);
        const auto container_max_size = alloc_max_size / sizeof(value_type);

        return container_max_size;
    }

    bool empty() {
        return begin() == end();
    }

    void clear() {
        DestructBuffer();
        traits::deallocate(allocator_, buffer_, capacity_);
        buffer_ = traits::allocate(allocator_, capacity_);
        ConstructBuffer();
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }

    void assign(iterator first, iterator second) {
        DestructBuffer();
        traits::deallocate(allocator_, buffer_, capacity_);
        if ((second - 1) - first < 0) {
            capacity_ = first.buffer_size_ + ((second - 1) - first) + 1;
        } else {
            capacity_ = ((second - 1) - first) + 1;
        }
        buffer_ = traits::allocate(allocator_, capacity_);
        ConstructBuffer();
        size_ = 0;
        head_ = 0;
        tail_ = 0;
        auto j = begin();
        for (auto i = first; i != second - 1; ++i) {
            *j = *i;
            ++size_;
            ++tail_;
            j = iterator(buffer_, size_, j.current_index_ + 1);
        }
        *(j) = *(second - 1);
        ++size_;
        ++tail_;
    }

    void assign(const std::initializer_list<value_type>& il) {
        DestructBuffer();
        traits::deallocate(allocator_, buffer_, capacity_);
        capacity_ = il.size();
        size_ = il.size();
        tail_ = il.size();
        buffer_ = traits::allocate(allocator_, capacity_);
        ConstructBuffer();
        std::copy(il.begin(), il.end(), buffer_);
    }

    void assign(int n, value_type t) {
        DestructBuffer();
        traits::deallocate(allocator_, buffer_, capacity_);
        capacity_ = n;
        buffer_ = traits::allocate(allocator_, capacity_);
        ConstructBuffer();
        size_ = n;
        head_ = 0;
        tail_ = n;
        std::fill(buffer_, buffer_ + n, t);
    }

    iterator insert(const_iterator p, value_type t) {
        for (int j = size_; j > p.current_index_; --j) {
            buffer_[j] = buffer_[j - 1];

        }
        ++size_;
        ++tail_;
        buffer_[p.current_index_] = t;

        return iterator(buffer_, size_, p.current_index_);
    }

    iterator insert(const_iterator p, size_t n, value_type t) {
        for (int i = 0; i < n; ++i) {
            for (int j = size_; j > p.current_index_; --j) {
                buffer_[j] = buffer_[j - 1];

            }
            ++size_;
            ++tail_;
            p = const_iterator(buffer_, size_, p.current_index_ + 1);
        }
        for (int i = 0; i < n; ++i) {
            buffer_[p.current_index_ - 1 - i] = t;
        }

        return iterator(buffer_, size_, p.current_index_ - n);
    }

    iterator insert(const_iterator p, const std::initializer_list<value_type>& il) {
        for (int k = 0; k < il.size(); ++k) {
            for (int m = size_; m > p.current_index_; --m) {
                buffer_[m] = buffer_[m - 1];
            }
            ++size_;
            ++tail_;
            p = const_iterator(buffer_, size_, p.current_index_ + 1);
        }
        auto it = il.end() - 1;
        for (int k = 0; k < il.size(); ++k) {
            buffer_[p.current_index_ - 1 - k] = *it;
            --it;
        }

        return iterator(buffer_, size_, p.current_index_ - il.size());
    }

    iterator insert(const_iterator p, iterator i, iterator j) {
        size_t capacity;
        if ((j - 1) - i < 0) {
            capacity = i.buffer_size_ + ((j - 1) - i) + 1;
        } else {
            capacity = ((j - 1) - i) + 1;
        }
        for (int k = 0; k < capacity; ++k) {
            for (int m = size_; m > p.current_index_; --m) {
                buffer_[m] = buffer_[m - 1];
            }
            ++size_;
            ++tail_;
            p = const_iterator(buffer_, size_, p.current_index_ + 1);
        }
        iterator it = j - 1;
        for (int k = 0; k < j - i; ++k) {
            buffer_[p.current_index_ - 1 - k] = *it;
            --it;
        }

        return iterator(buffer_, size_, p - (j - i));
    }

    iterator erase(const_iterator q) {
        for (int i = q.current_index_; i < size_ - 1; i++) {
            buffer_[i] = buffer_[i + 1];
        }
        --size_;
        --tail_;

        return iterator(buffer_, size_, q.current_index_);
    }

    iterator erase(const_iterator q1, const_iterator q2) {
        size_t capacity;
        if ((q2 - 1) - q1 < 0) {
            capacity = q1.buffer_size_ + ((q2 - 1) - q1) + 1;
        } else {
            capacity = ((q2 - 1) - q1) + 1;
        }
        for (int i = 0; i < capacity; ++i) {
            for (int j = q1.current_index_; j < size_ - 1; j++) {
                buffer_[j] = buffer_[j + 1];
            }
            --size_;
            --tail_;
        }

        return iterator(buffer_, size_, q1.current_index_);
    }

    reference front() {
        return *begin();
    }

    reference back() {
        auto tmp = end();
        --tmp;

        return *tmp;
    }

    reference at(size_t n) {
        return (*this)[n];
    }

    void push_back(const_reference t) {
        if (end().current_index_ == capacity_) {
            tail_ = 0;
        }
        if (tail_ == head_) {
            rewrite_ = true;
        }
        *(end()) = t;
        ++tail_;
        if (rewrite_) {
            head_ = tail_;
        }
        if (size_ < capacity_) {
            ++size_;
        }
    }

    void push_front(const_reference t) {
        if (size_ < capacity_) {
            ++size_;
            *(end()) = t;
            ++tail_;
        } else {
            *(end() - 1) = t;
        }
        if (head_ == 0) {
            head_ = size_ - 1;
        } else {
            --head_;
        }
        --tail_;
        if (tail_ == 0) {
            tail_ = size_;
            rewrite_ = false;
        }

    }

private:
    void ConstructBuffer() {
        for (int i = 0; i < capacity_; ++i) {
            traits::construct(allocator_, buffer_ + i);
        }
    }

    void DestructBuffer() {
        for (int i = 0; i < capacity_; ++i) {
            traits::destroy(allocator_, buffer_ + i);
        }
    }

    Allocator allocator_;
    bool rewrite_ = false;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t capacity_;
    size_t size_ = 0;
    value_type* buffer_ = nullptr;
};

template<typename value_type>
void swap(CCirtucalBuffer<value_type>& first, CCirtucalBuffer<value_type>& second) {
    first.swap(second);
}
