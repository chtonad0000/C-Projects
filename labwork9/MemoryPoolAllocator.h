#pragma once

#include "memory_pool.h"

#include <stdexcept>
#include <vector>


template<typename T>
class MemoryPoolAllocator {
public:
    using propogate_on_container_copy_assignment = std::true_type;
    using propogate_on_container_move_assignment = std::true_type;
    using propogate_on_container_swap = std::true_type;
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;

    template<class U> friend
    class MemoryPoolAllocator;

    explicit MemoryPoolAllocator() = default;

    explicit MemoryPoolAllocator(std::vector<Pool>& pools) : pools_(&pools) {
        std::sort(pools_->begin(), pools_->end());
    }

    constexpr MemoryPoolAllocator(const MemoryPoolAllocator& other) noexcept {
        pools_ = other.pools_;
    }

    template<typename U>
    constexpr explicit MemoryPoolAllocator(const MemoryPoolAllocator<U>& other) noexcept {
        pools_ = other.pools_;
    }

    template<class U>
    struct rebind {
        using other = MemoryPoolAllocator<U>;
    };

    MemoryPoolAllocator& operator=(const MemoryPoolAllocator& other) {
        this->pools_ = other.pools_;

        return *this;
    }

    pointer allocate(size_type count_objects) {
        size_type n = count_objects * sizeof(value_type);
        for (int i = 0; i < pools_->size(); ++i) {
            if (!(*pools_)[i].AllocPossibility(n)) {
                continue;
            }
            auto new_ptr = static_cast<pointer>((*pools_)[i].alloc(n));

            return new_ptr;
        }

        throw std::bad_alloc();
    }

    void deallocate(pointer p, size_type n) {
        for (int i = 0; i < pools_->size(); ++i) {
            if ((*pools_)[i].has_pointer(static_cast<void*>(p))) {
                (*pools_)[i].dealloc(static_cast<void*>(p), n * sizeof(T));
            }
        }
    }

    bool operator==(const MemoryPoolAllocator<T>& other) {
        return this->pools_ == other.pools_;
    }


    bool operator!=(const MemoryPoolAllocator<T>& other) {
        return !(*this == other);
    }

    constexpr ~MemoryPoolAllocator() = default;

private:
    std::vector<Pool>* pools_;
};